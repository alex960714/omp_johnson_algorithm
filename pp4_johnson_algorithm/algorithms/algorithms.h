#ifndef _ALGORITHMS_H_
#define _ALGORITHMS_H_

#include <list>
using namespace std;

struct edge
{
	int node;
	int weight;
};

bool Bellman_Ford(list<edge> *v, int vert_num, int vert, int *dist);
bool Bellman_Ford(int* vert_disp, int *vert_adj, int *edges, int vert_num, int vert, int *dist);

void Dijkstra(list<edge> *v, int vert_num, int vert, int *dist, int *delta);
void Dijkstra(int* vert_disp, int *vert_adj, int *edges, int vert_num, int vert, int *dist, int *delta);

#endif
