#include <stdio.h>
#include "algorithms.h"
#include <list>
#include <time.h>
#include <fstream>
#include <omp.h>
using namespace std;

int vert_num, coeff, edges_num;
int *pre_dist, *delta, **dist_par;
list<edge> *edges;

double par_time_st, par_time_en;

void mem_init();
void generate_graph();
void read_graph_from_txt();
void count_edges1();
void count_edges2(int *curr_dist, int vert);
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
	coeff = atoi(argv[2]);
	/*vert_num = 2000;
	coeff = 1;*/

	mem_init();
	generate_graph();

	par_time_st = omp_get_wtime();
	if (!Bellman_Ford(edges, vert_num + 1, vert_num, delta))
	{
		printf("\nThere is negative cycle in graph\n");
		exit(0);
	}
	count_edges1();
	int i;
#pragma omp parallel
	{
		#pragma omp for
		for (i = 0; i < vert_num; i++)
		{
			Dijkstra(edges, vert_num, i, dist_par[i]);
			count_edges2(dist_par[i], i);
		}
	}
	par_time_en = omp_get_wtime();
	check_results();

	del_mem();
	return 0;
}

void mem_init()
{
	edges = new list<edge>[vert_num + 1];
	pre_dist = new int[vert_num + 1];
	delta = new int[vert_num + 1];
	dist_par = new int*[vert_num];
	for (int i = 0; i < vert_num; i++)
		dist_par[i] = new int[vert_num];
}

void generate_graph()
{
	int value;
	edges_num = 0;
	//edge *curr_edge;
	srand(time(nullptr));
	
	//if (argc == 3)
		//read_graph_from_txt();
	/*else
	{*/
		for (int i = 0; i < vert_num; i++)
		{
			for (int j = 0; j < vert_num; j++)
			{
				value = rand() % 1000;
				if (value <= coeff && i != j)
				{
					edges[i].push_back({ j,rand()%10 - 1 });
					edges_num++;
				}
			}
		}
	//}
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
#pragma omp parallel for private(it)
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

bool check_results()
{
	if (vert_num < 20)
	{
		
		printf("\n\nDistances:\n");
		for (int i = 0; i < vert_num; i++)
		{
			for (int j = 0; j < vert_num; j++)
				printf("%d ", dist_par[i][j]);
			printf("\n");
		}
	}
	printf("Vertexes: %d\nEdges: %d\n", vert_num, edges_num);
	printf("Parallel version time: %f\n", par_time_en - par_time_st);
	return true;
}

void del_mem()
{
	for (int i = 0; i < vert_num; i++)
		delete[] dist_par[i];
	delete[] dist_par;
	delete[] pre_dist;
	delete[] delta;
	delete[] edges;
}