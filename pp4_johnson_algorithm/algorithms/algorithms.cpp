#include "algorithms.h"
#include "d_heap.h"
#include <omp.h>

bool Bellman_Ford(list<edge> *v, int vert_num, int vert, int *dist)
{
	int curr_edge;
	list<edge>::iterator it;

	for (int i = 0; i < vert_num; i++)
		dist[i] = INT_MAX;
	dist[vert] = 0;

	for (int i = 0; i < vert_num - 1; i++)
	{
		//int j;
//#pragma omp parallel private(j) shared(dist, v)
		//{
//#pragma omp for
			for (int j = 0; j < vert_num; j++)
			{
				if (dist[j] != INT_MAX)
				{
					for (it = v[j].begin(); it != v[j].end(); ++it)
					{
						curr_edge = dist[j] + it->weight;
//#pragma omp critical
						if (dist[it->node] > curr_edge)
							dist[it->node] = curr_edge;
					}
				}
			}
		//}
	}

	for (int j = 0; j < vert_num; j++)
	{
		for (it = v[j].begin(); it != v[j].end(); ++it)
		{
			curr_edge = dist[j] + it->weight;
			if (dist[it->node]>curr_edge)
				return false;
		}
	}
	return true;
}

bool Bellman_Ford(int * vert_disp, int * vert_adj, int * edges, int vert_num, int vert, int * dist)
{
	int curr_edge;

	for (int i = 0; i < vert_num; i++)
		dist[i] = INT_MAX;
	dist[vert] = 0;

	for (int i = 0; i < vert_num - 1; i++)
	{
		for (int j = 0; j < vert_num; j++)
		{
			if (dist[j] != INT_MAX)
			{
				for (int k = vert_disp[j]; k < vert_disp[j+1]; k++)
				{
					curr_edge = dist[j] + edges[k];
					if (dist[vert_adj[k]] > curr_edge)
						dist[vert_adj[k]] = curr_edge;
				}
			}
		}
	}

	for (int j = 0; j < vert_num; j++)
	{
		for (int k = vert_disp[j]; k < vert_disp[j + 1]; k++)
		{
			curr_edge = dist[j] + edges[k];
			if (dist[vert_adj[k]] > curr_edge)
				return false;
		}
	}
	return true;
}

void Dijkstra(list<edge> *v, int vert_num, int vert, int *dist, int *delta)
{
	d_heap Q;
	d_node curr_node;
	list<edge>::iterator it;
	int curr_vert, curr_edge;

	for (int i = 0; i < vert_num; i++)
		dist[i] = INT_MAX;
	dist[vert] = 0;

	Q.MakeHeap(dist, vert_num);
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
				}
			}
		}
	}
	for (int j = 0; j < vert_num; j++)
	{
		dist[j] += (delta[j] - delta[vert]);
	}
}

void Dijkstra(int * vert_disp, int * vert_adj, int * edges, int vert_num, int vert, int * dist, int * delta)
{
	d_heap Q;
	d_node curr_node;
	int curr_vert, curr_edge;

	for (int i = 0; i < vert_num; i++)
		dist[i] = INT_MAX;
	dist[vert] = 0;

	Q.MakeHeap(dist, vert_num);
	while (!Q.IsEmpty())
	{
		curr_node = Q.DeleteMin();
		curr_vert = curr_node.node;
		if (dist[curr_vert] != INT_MAX)
		{
			for (int i = vert_disp[curr_vert]; i < vert_disp[curr_vert+1]; i++)
			{
				curr_edge = dist[curr_vert] + edges[i];
				if (dist[vert_adj[i]] > curr_edge)
				{
					Q.DecreaseWeight(vert_adj[i], dist[vert_adj[i]] - curr_edge);
					dist[vert_adj[i]] = curr_edge;
				}
			}
		}
	}
	/*for (int j = 0; j < vert_num; j++)
	{
		dist[j] += (delta[j] - delta[vert]);
	}*/
}
