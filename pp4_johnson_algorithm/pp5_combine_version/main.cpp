#include <stdio.h>
#include "tbb\task_scheduler_init.h"
#include "tbb\parallel_for.h"
#include "tbb\tick_count.h"
#include "algorithms.h"
#include "dijkstra_functor.h"
#include <list>
#include <time.h>
#include <fstream>
using namespace std;

int vert_num, coeff, edges_num;
int *delta, **dist_seq, **dist_par;
list<edge> *edges;

void mem_init();
void generate_graph();
void read_graph_from_txt();
void count_edges1();							//recount edges for Dijkstra algorithm
void count_edges2(int *curr_dist, int vert);	//recount counted distances for initial conditions
void graph_recovery();
bool check_results();
void del_mem();

int main(int argc, char **argv)
{
	if (argc < 3)
	{
		printf("So few arguments\n");
		exit(0);
	}
	vert_num = atoi(argv[1]);
	coeff = atoi(argv[2]) - 1;
	//vert_num = 10;
	//coeff = 500;

	mem_init();
	generate_graph();

	tbb::task_scheduler_init init(4);
	tbb::tick_count time;

	tbb::tick_count seq_time_st = tbb::tick_count::now();
	if (!Bellman_Ford(edges, vert_num + 1, vert_num, delta))
	{
		printf("\nThere is negative cycle in graph\n");
		exit(0);
	}

	count_edges1();

	int i;
	{
		for (i = 0; i < vert_num; i++)
		{
			Dijkstra(edges, vert_num, i, dist_seq[i], delta);
			//count_edges2(dist_par[i], i);
		}
	}
	tbb::tick_count seq_time_en = tbb::tick_count::now();

	graph_recovery();

	tbb::tick_count par_time_st = tbb::tick_count::now();
	if (!Bellman_Ford(edges, vert_num + 1, vert_num, delta))
	{
		printf("\nThere is negative cycle in graph\n");
		exit(0);
	}

	count_edges1();

	tbb::parallel_for(tbb::blocked_range<int>(0, vert_num), dij_functor(vert_num, dist_par, delta, edges));

	tbb::tick_count par_time_en = tbb::tick_count::now();

	if (vert_num < 20)
	{
		printf("\n\nDistances (sequential version):\n");
		for (int i = 0; i < vert_num; i++)
		{
			for (int j = 0; j < vert_num; j++)
				printf("%d ", dist_seq[i][j]);
			printf("\n");
		}
		printf("\n\nDistances (parallel version):\n");
		for (int i = 0; i < vert_num; i++)
		{
			for (int j = 0; j < vert_num; j++)
				printf("%d ", dist_par[i][j]);
			printf("\n");
		}
	}
	printf("Vertexes: %d\nEdges: %d\n", vert_num, edges_num);
	printf("Sequential version time: %f\n", (seq_time_en - seq_time_st).seconds());
	printf("Parallel version time: %f\n", (par_time_en - par_time_st).seconds());
	printf("Acceleration: %f\n", (seq_time_en - seq_time_st).seconds() / (par_time_en - par_time_st).seconds());
	if (check_results())
		printf("Results are equal\n");
	else
		printf("Results are not equal\n");
	

	del_mem();
	return 0;
}

void mem_init()
{
	edges = new list<edge>[vert_num + 1];
	delta = new int[vert_num + 1];

	dist_seq = new int*[vert_num];
	for (int i = 0; i < vert_num; i++)
		dist_seq[i] = new int[vert_num];

	dist_par = new int*[vert_num];
	for (int i = 0; i < vert_num; i++)
		dist_par[i] = new int[vert_num];
}

void generate_graph()
{
	int value;
	edges_num = 0;
	srand(time(nullptr));

	for (int i = 0; i < vert_num; i++)
	{
		for (int j = 0; j < vert_num; j++)
		{
			value = rand() % 1000;
			if (value <= coeff && i != j)
			{
				edges[i].push_back({ j,rand() % 1000 - 10 });
				edges_num++;
			}
		}
	}
	for (int i = 0; i < vert_num; i++)
	{
		if (i != vert_num)
			edges[vert_num].push_back({ i,0 });
	}

	if (vert_num < 20)
	{
		printf("Adjacency lists:");
		list<edge>::iterator it;
		for (int i = 0; i < vert_num; i++)
		{
			printf("\n%d:", i);
			for (it = edges[i].begin(); it != edges[i].end(); ++it)
			{
				printf(" {%d, %d} ", (*it).node, (*it).weight);
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
	list<edge>::iterator it;
	for (int i = 0; i < vert_num; i++)
	{
		for (it = edges[i].begin(); it != edges[i].end(); ++it)
		{
			(*it).weight += (delta[i] - delta[(*it).node]);
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
	list<edge>::iterator it;
	for (int i = 0; i < vert_num; i++)
	{
		for (it = edges[i].begin(); it != edges[i].end(); ++it)
		{
			(*it).weight -= (delta[i] - delta[(*it).node]);
		}
	}

	delete[] delta;
	delta = new int[vert_num + 1];
}

bool check_results()
{
	for (int i = 0; i < vert_num; i++)
		for (int j = 0; j < vert_num; j++)
			if (dist_seq[i][j] != dist_par[i][j])
				return false;
	return true;
}

void del_mem()
{
	for (int i = 0; i < vert_num; i++)
		delete[] dist_par[i];
	delete[] dist_par;
	delete[] delta;
	delete[] edges;
}