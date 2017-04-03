#ifndef _DIJKSTRA_FUNCTOR_H_
#define _DIJKSTRA_FUNCTOR_H_

#include "tbb\task_scheduler_init.h"
#include "tbb\parallel_for.h"
#include "algorithms.h"
#include <list>
using namespace std;

class dij_functor
{
	int vert_num;
	int* delta;
	int** const dist;
	list<edge> *edges;
public:
	dij_functor(int vn, int** d, int* del, list<edge> *edg): vert_num(vn), delta(del), dist(d), edges(edg)
	{
	}

	void operator()(const tbb::blocked_range<int> &iter) const
	{
		for (int i = iter.begin(); i != iter.end(); i++)
		{
			Dijkstra(edges, vert_num, i, dist[i]);
			for (int j = 0; j < vert_num; j++)
			{
				dist[j] += (delta[j] - delta[i]);
			}
		}
	}
};

#endif
