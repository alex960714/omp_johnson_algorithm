#include "algorithms.h"
#include "d_heap.h"

bool Bellman_Ford(list<edge> *v, int vert_num, int vert, int *dist)
{
	int curr_edge;
	list<edge>::iterator it;
	dist[vert] = 0;
	for (int i = 0; i < vert_num; i++)
		if (i != vert)
			dist[i] = INT_MAX;
	for (int i = 0; i < vert_num - 1; i++)
	{
		for (int j = 0; j < vert_num; j++)
		{
			/*for (int k = 0; k < vert_num; k++)
			{*/
			for (it = v[j].begin(); it != v[j].end(); ++it)
			{
				curr_edge = dist[j] + it->weight;
				if (dist[it->node]>curr_edge)
					dist[it->node] = curr_edge;
			}
		}
	}

	for (int j = 0; j < vert_num; j++)
	{
		/*for (int k = 0; k < vert_num; k++)
		{
		curr_edge = dist[j] + matr[j][k];
		if (dist[k] > curr_edge)
		return false;
		}*/
		for (it = v[j].begin(); it != v[j].end(); ++it)
		{
			curr_edge = dist[j] + it->weight;
			if (dist[it->node]>curr_edge)
				return false;
		}
	}
	return true;
}

void Dijkstra(list<edge> *v, int size, int vert, int *dist)
{

}