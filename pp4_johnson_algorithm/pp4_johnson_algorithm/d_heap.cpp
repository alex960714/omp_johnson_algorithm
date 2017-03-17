#include "d_heap.h"

void d_heap::Swap(int node1, int node2)
{
	int tmp_w = weights[vert_pointer[node1]].weight;
	int tmp_n = vert_pointer[node1];

	weights[tmp_n].weight = weights[vert_pointer[node2]].weight;
	weights[tmp_n].node=node2;

	weights[vert_pointer[node2]].weight = tmp_w;
	weights[vert_pointer[node2]].node = node1;

	vert_pointer[node1] = vert_pointer[node2];
	vert_pointer[node2] = tmp_n;
}

void d_heap::Diving(int node)
{
	int curr_node = node;
	int child = GetMinChild(node);
	while (child != -1 && weights[vert_pointer[curr_node]].weight > weights[vert_pointer[child]].weight)
	{
		Swap(curr_node, child);
		child = GetMinChild(curr_node);
	}
}

void d_heap::Emersion(int node)
{
	int curr_node = node;
	int parent = GetParent(node);
	while (parent != -1 && weights[vert_pointer[curr_node]].weight < weights[vert_pointer[parent]].weight)
	{
		Swap(curr_node, parent);
		parent = GetParent(curr_node);
	}
}

int d_heap::GetLeft(int node)
{
	int tmp = vert_pointer[node]*d + 1;
	if (tmp >= power)
		return -1;
	return weights[tmp].node;
}

int d_heap::GetRight(int node)
{
	int tmp = GetLeft(node);
	if (tmp == -1)
		return -1;
	tmp = vert_pointer[tmp] + (d - 1);
	if (tmp >= power - 1)
		return weights[power - 1].node;
	return weights[tmp].node;
}

int d_heap::GetParent(int node)
{
	int tmp = vert_pointer[node];
	if (!tmp)
		return -1;
	return weights[(tmp - 1) / d].node;
}

int d_heap::GetMinChild(int node)
{
	int child1 = GetLeft(node);
	if (child1 == -1)
		return -1;

	int min = weights[vert_pointer[child1]].weight;
	int min_node = vert_pointer[child1];

	for (int i = vert_pointer[child1] + 1; i <= vert_pointer[GetRight(node)]; i++)
	{
		if (min > weights[i].weight)
		{
			min = weights[i].weight;
			min_node = i;
		}
	}
	return weights[min_node].node;
}

void d_heap::Insert(int node, int w)
{
	

}

void d_heap::DecreaseWeight(int node, int delta)
{
	weights[vert_pointer[node]].weight -= delta;
	if (delta > 0)
		Emersion(node);
	else
		Diving(node);
}

d_node d_heap::DeleteMin()
{
	Swap(weights[0].node, weights[power - 1].node);
	power--;
	Diving(weights[0].node);
	return weights[power];
}

d_node d_heap::Delete(int node)
{
	DecreaseWeight(node, weights[vert_pointer[node]].weight+1);
	//DecreaseWeight(node, INT_MAX);
	return DeleteMin();
}

void d_heap::MakeHeap(int *w, int num)
{
	weights = new d_node[num];
	vert_pointer = new int[num];

	for (int i = 0; i < num; i++)
	{
		weights[i].node = i;
		weights[i].weight = w[i];
		vert_pointer[i] = i;
	}
	power = num;

	for (int i = power - 1; i >= 0; i--)
		Diving(i);
}