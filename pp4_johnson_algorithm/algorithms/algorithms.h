#ifndef _ALGORITHMS_H_
#define _ALGORITHMS_H_

#include <list>
using namespace std;

struct edge
{
	int node;
	int weight;
};

bool Bellman_Ford(list<edge> *v, int vert_num, int vert, int *dist_par);
void Dijkstra(list<edge> *v, int vert_num, int vert, int *dist_par, int *delta);

#endif
