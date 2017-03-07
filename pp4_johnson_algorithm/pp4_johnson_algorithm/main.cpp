#include <stdio.h>
#include "algorithms.h"
#include <list>
using namespace std;

void generate_graph(list<edge> *edges, int vert_num, int coeff);
void count_edges1(list<edge> *edges, int vert_num, int *dist, int *delta);
void count_edges2(int *dist, int vert_num, int *delta);
void check_results(int **dist, int vert_num);

int main(int argc, char **argv)
{
	int vert_num, coeff;
	int *pre_dist, *delta, **dist;
	list<edge> *edges;

	if (argc < 3)
	{
		printf("So few arguments\n");
		exit(0);
	}
	vert_num = atoi(argv[1]);
	coeff = atoi(argv[2]);

	generate_graph(edges, vert_num, coeff);
	if (!Bellman_Ford(edges, vert_num + 1, vert_num, pre_dist))
	{
		printf("There is negative cycle in graph\n");
		exit(0);
	}
	count_edges1(edges, vert_num, pre_dist, delta);

	for (int i = 0; i < vert_num; i++)
	{
		Dijkstra(edges, vert_num, i, dist[i]);
		count_edges2(dist[i], vert_num, delta);
	}

	check_results(dist, vert_num);

	for (int i = 0; i < vert_num; i++)
		delete[] dist[i];
	delete[] dist;
	delete[] pre_dist;
	delete[] delta;
	delete[] edges;
	return 0;
}