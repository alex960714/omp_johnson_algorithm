#include <stdlib.h>

bool Bellman_Ford(int **matr, int vert_num, int vert, int *dist)
{
	int curr_edge;
	dist[vert] = 0;
	for (int i = 0; i < vert_num; i++)
		if (i != vert)
			dist[i] = INT_MAX;
	for (int i = 0; i < vert_num - 1; i++)
	{
		for (int j = 0; j < vert_num; j++)
		{
			for (int k = 0; k < vert_num; k++)
			{
				curr_edge = dist[j] + matr[j][k];
				if (dist[k]>curr_edge)
					dist[k] = curr_edge;
			}
		}
	}

	for (int j = 0; j < vert_num; j++)
	{
		for (int k = 0; k < vert_num; k++)
		{
			curr_edge = dist[j] + matr[j][k];
			if (dist[k] > curr_edge)
				return false;
		}
	}
	return true;
}