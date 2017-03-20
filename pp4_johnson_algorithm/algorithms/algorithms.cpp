#include "algorithms.h"
#include "d_heap.h"

bool Bellman_Ford(list<edge> *v, int vert_num, int vert, int *dist)
{
	int curr_edge;
	list<edge>::iterator it;

	for (int i = 0; i < vert_num; i++)
		dist[i] = INT_MAX;
	dist[vert] = 0;

	for (int i = 0; i < vert_num - 1; i++)
	{
		for (int j = 0; j < vert_num; j++)
		{
			/*for (int k = 0; k < vert_num; k++)
			{*/
			if (dist[j] != INT_MAX)
			{
				for (it = v[j].begin(); it != v[j].end(); ++it)
				{
					curr_edge = dist[j] + it->weight;
					if (dist[it->node] > curr_edge)
						dist[it->node] = curr_edge;
				}
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

void Dijkstra(list<edge> *v, int vert_num, int vert, int *dist)
{
	d_heap Q;
	d_node curr_node;
	list<edge>::iterator it;
	int curr_vert, curr_edge;

	for (int i = 0; i < vert_num; i++)
		dist[i] = INT_MAX;
	dist[vert] = 0;

	Q.MakeHeap(dist, vert_num);
	//Q.Insert(vert, dist[vert]);
	while (!Q.IsEmpty())
	{
		curr_node = Q.DeleteMin();
		curr_vert = curr_node.node;
		if (dist[curr_vert] != INT_MAX)
		{
			for (it = v[curr_vert].begin(); it != v[curr_vert].end(); ++it)
			{
				curr_edge = dist[curr_vert] + it->weight;
				if (dist[it->node] > curr_edge)
				{
					Q.DecreaseWeight(it->node, dist[it->node] - curr_edge);
					dist[it->node] = curr_edge;
					//Q.Delete(it->node);
					//Q.Insert(it->node, dist[it->node]);

				}
			}
		}
	}
}