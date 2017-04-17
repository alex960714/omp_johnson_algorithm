#include <stdio.h>
#include "algorithms.h"
#include <list>
#include <time.h>
#include <fstream>
#include <omp.h>
#include "mpi.h"
using namespace std;

int vert_num, coeff, edges_num;
int *delta=nullptr, *dist_seq=nullptr, *dist_send=nullptr, *dist_par=nullptr, *vert_disp=nullptr, *vert_adj=nullptr, *edg=nullptr;
list<edge> *edges=nullptr;
int *sendcounts=nullptr, *displs=nullptr;

double seq_time_st, seq_time_en, par_time_st, par_time_en;
int ProcNum, ProcRank;
int isNegativeCycle;

void mem_init();
void generate_graph();
void read_graph_from_txt();
void count_edges1();							//recount edges for Dijkstra algorithm
void count_edges2(int *curr_dist, int vert);	//recount counted distances for initial conditions
void graph_recovery();
bool check_results();
void del_mem();
void print_arrays();

int main(int argc, char **argv)
{
	if (argc < 3)
	{
		printf("So few arguments\n");
		exit(0);
	}
	vert_num = atoi(argv[1]);
	coeff = atoi(argv[2]) - 1;

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

	mem_init();

	MPI_Bcast(sendcounts, ProcNum, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(displs, ProcNum, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(vert_disp, vert_num + 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(vert_adj, vert_num + edges_num, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(edg, vert_num + edges_num, MPI_INT, 0, MPI_COMM_WORLD);

	/*if(!ProcRank)
		print_arrays();
	MPI_Finalize();
	exit(0);*/

	//parallel version	

	par_time_st = MPI_Wtime();
	if (!ProcRank)
	{
		if (!Bellman_Ford(vert_disp, vert_adj, edg, vert_num + 1, vert_num, delta))
		{
			isNegativeCycle = 1;
			printf("\nThere is negative cycle in graph\n");
		}
		else
		{
			isNegativeCycle = 0;
			count_edges1();
			printf("\n\n");
			for (int i = 0; i < vert_num + 1; i++)
				printf("%d ", delta[i]);
			printf("\n");
		}
	}

	MPI_Bcast(&isNegativeCycle, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (!isNegativeCycle)
	{
		MPI_Bcast(delta, vert_num + 1, MPI_INT, 0, MPI_COMM_WORLD);
		int i, vert_num_local, start_local;
		omp_set_num_threads(4);

		vert_num_local = sendcounts[ProcRank] / vert_num;
		start_local = displs[ProcRank] / vert_num;
#pragma omp parallel shared(vert_disp, vert_adj, edg, dist_send, vert_num, vert_num_local, start_local) private(i)
		{
#pragma omp for
			for (i = 0; i < vert_num_local; i++)
			{
				Dijkstra(vert_disp, vert_adj, edg, vert_num, start_local + i, dist_send + i*vert_num, delta);
			}
		}
		MPI_Gatherv(dist_send, sendcounts[ProcRank], MPI_INT, dist_par, sendcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);
		par_time_en = MPI_Wtime();

		//end of parallel version

		if (!ProcRank)
		{
			if (vert_num < 20)
			{
				printf("\n\nDistances (parallel version):\n");
				for (int i = 0; i < vert_num; i++)
				{
					for (int j = 0; j < vert_num; j++)
						printf("%d ", dist_par[i*vert_num + j]);
					printf("\n");
				}
			}
			printf("Vertexes: %d\nEdges: %d\n", vert_num, edges_num);
			printf("Parallel version time: %f\n", par_time_en - par_time_st);
		}
	}
	del_mem();
	MPI_Finalize();
	return 0;
}

void mem_init()
{
	sendcounts = new int[ProcNum];
	displs = new int[ProcNum];
	if (!ProcRank)
	{
		generate_graph();
		dist_par = new int[vert_num*vert_num];

		int vert_proc = vert_num / ProcNum;
		int vert_add = vert_num % ProcNum;
		//sendcounts[0] = 0;
		displs[0] = 0;
		for (int i = 0; i < vert_add; i++)
		{
			sendcounts[i] = (vert_proc + 1) * vert_num;
			if (i)
				displs[i] = sendcounts[i - 1] + displs[i - 1];
		}
		for (int i = vert_add; i < ProcNum; i++)
		{
			sendcounts[i] = vert_proc * vert_num;
			if (i)
				displs[i] = sendcounts[i - 1] + displs[i - 1];
		}
	}
	
	MPI_Bcast(&edges_num, 1, MPI_INT, 0, MPI_COMM_WORLD);
	delta = new int[vert_num + 1];
	dist_send = new int[sendcounts[ProcRank]];
	if (ProcRank)
	{
		vert_disp = new int[vert_num + 1];
		vert_adj = new int[edges_num + vert_num];
		edg = new int[edges_num + vert_num];
	}
}

void generate_graph()
{
	int value;
	edges_num = 0;
	srand(time(nullptr));

	edges = new list<edge>[vert_num + 1];
	for (int i = 0; i < vert_num; i++)
	{
		for (int j = 0; j < vert_num; j++)
		{
			value = rand() % 1000;
			if (value <= coeff && i != j)
			{
				edges[i].push_back({ j,rand() % 10 - 1 });
				edges_num++;
			}
		}
	}
	vert_disp = new int[vert_num + 2];
	vert_adj = new int[edges_num + vert_num];
	edg = new int[edges_num + vert_num];

	list<edge>::iterator it;
	int j = 0;
	for (int i = 0; i <= vert_num; i++)
	{
		vert_disp[i] = j;
		for (it = edges[i].begin(); it != edges[i].end(); ++it, j++)
		{
			vert_adj[j] = it->node;
			edg[j] = it->weight;
		}
	}
	vert_disp[vert_num] = j;
	for (int i = 0; i < vert_num; i++, j++)
	{
		vert_adj[j] = i;
		edg[j] = 0;
	}
	vert_disp[vert_num + 1] = vert_disp[vert_num] + vert_num;

	delete[] edges;
	
	if (vert_num < 20)
	{
		printf("Adjacency lists:");
		for (int i = 0; i < vert_num; i++)
		{
			printf("\n%d:", i);
			for (int j = vert_disp[i]; j < vert_disp[i+1]; j++)
			{
				printf(" {%d, %d} ", vert_adj[j], edg[j]);
			}
		}
	}
}

void read_graph_from_txt()
{
	int buf1, buf2;
	ifstream is;
	is.open("graph.txt");
	for (int i = 0; i < vert_num; i++)
	{
		for (int j = 0; j < vert_num; j++)
		{
			is >> buf1;
			if (buf1 == -1)
				break;
			is >> buf2;
			edges[i].push_back({ buf1, buf2 });
		}
	}
	is.close();
}

void count_edges1()
{
	for (int i = 0; i < vert_num; i++)
	{
		for (int j = vert_disp[i]; j < vert_disp[i+1]; j++)
		{
			edg[j] += (delta[i] - delta[vert_adj[j]]);
		}
	}
}

void count_edges2(int *curr_dist, int vert)
{
	for (int i = 0; i < vert_num; i++)
	{
		curr_dist[i] += (delta[i] - delta[vert]);
	}
}

void graph_recovery()
{
	for (int i = 0; i < vert_num; i++)
	{
		for (int j = vert_disp[i]; j < vert_disp[i + 1]; j++)
		{
			edg[vert_adj[j]] -= (delta[i] - delta[vert_adj[j]]);
		}
	}

	delete[] delta;
	delta = new int[vert_num + 1];
}

/*bool check_results()
{
	for (int i = 0; i < vert_num; i++)
		for (int j = 0; j < vert_num; j++)
			if (dist_seq[i][j] != dist_par[i][j])
				return false;
	return true;
}*/

void del_mem()
{
	delete[] dist_par;
	delete[] delta;
}

void print_arrays()
{
	printf("\nVertexes num: %d\n",vert_num);
	printf("Edges num: %d\n", edges_num);
	for (int i = 0; i < ProcNum; i++)
		printf("%d ", sendcounts[i]);
	printf("\n");
	for (int i = 0; i < ProcNum; i++)
		printf("%d ", displs[i]);
	printf("\n");
	for (int i = 0; i < vert_num + 1; i++)
		printf("%d ", vert_disp[i]);
	printf("\n");
	for (int i = 0; i < vert_num + edges_num; i++)
		printf("%d ", vert_adj[i]);
	printf("\n");
	for (int i = 0; i < vert_num + edges_num; i++)
		printf("%d ", edg[i]);
	printf("\n");
}