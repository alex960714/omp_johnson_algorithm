#ifndef _DIJKSTRA_FUNCTOR_H_
#define _DIJKSTRA_FUNCTOR_H_

#include "tbb\task_scheduler_init.h"
#include "tbb\parallel_for.h"
#include "algorithms.h"
#include <list>
using namespace std;

class dij_functor
{
	int* delta;
	int** const dist;
	list<edge> *edges;
public:
	dij_functor(int** d, int* del, list<edge> *edg): delta(del), dist(d), edges(edg)
	{
	}

	void operator()(const tbb::blocked_range<int> &iter) const
	{
		for (int i = iter.begin(); i != iter.end(); i++)
		{
			Dijkstra(edges, iter.end(), i, dist[i]);
			for (int j = 0; j < iter.end(); j++)
			{
				dist[j] += (delta[j] - delta[i]);
			}
		}
	}
};

#endif
