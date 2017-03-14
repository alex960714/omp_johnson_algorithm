#include <stdio.h>
#include "algorithms.h"
#include <list>
#include <time.h>
using namespace std;

int vert_num, coeff;
int *pre_dist, *delta, **dist;
list<edge> *edges;

void generate_graph();
void count_edges1();
void count_edges2(int *curr_dist, int vert);
bool check_results();
void del_mem();

int main(int argc, char **argv)
{
	/*if (argc < 3)
	{
		printf("So few arguments\n");
		exit(0);
	}
	vert_num = atoi(argv[1]);
	coeff = atoi(argv[2]);*/
	vert_num = 10;
	coeff = 50;

	generate_graph();
	if (!Bellman_Ford(edges, vert_num + 1, vert_num, delta))
	{
		printf("There is negative cycle in graph\n");
		exit(0);
	}
	count_edges1();

	for (int i = 0; i < vert_num; i++)
	{
		Dijkstra(edges, vert_num, i, dist[i]);
		count_edges2(dist[i],i);
	}

	check_results();

	del_mem();
	return 0;
}

void generate_graph()
{
	int value;
	//edge *curr_edge;
	srand(time(nullptr));
	edges = new list<edge>[vert_num + 1];
	pre_dist = new int[vert_num + 1];
	delta = new int[vert_num + 1];
	dist = new int*[vert_num];
	for (int i = 0; i < vert_num; i++)
		dist[i] = new int[vert_num];

	for (int i = 0; i < vert_num; i++)
	{
		for (int j = 0; j < vert_num; j++)
		{
			value = rand() % 100;
			if (value <= coeff && i != j)
			{
				/*curr_edge = new edge();
				curr_edge->node = j;
				curr_edge->weight = value;*/
				edges[i].push_back({j,value});
			}
		}
	}
	for (int i = 0; i < vert_num; i++)
	{
		if (i != vert_num)
			edges[vert_num].push_back({ i,0 });
	}
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

bool check_results()
{
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
		printf("\n\nDistances:\n");
		for (int i = 0; i < vert_num; i++)
		{
			for (int j = 0; j < vert_num; j++)
				printf("%d ", dist[i][j]);
			printf("\n");
		}
	}
	return true;
}

void del_mem()
{
	for (int i = 0; i < vert_num; i++)
		delete[] dist[i];
	delete[] dist;
	delete[] pre_dist;
	delete[] delta;
	delete[] edges;
}