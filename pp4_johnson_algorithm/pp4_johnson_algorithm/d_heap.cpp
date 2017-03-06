#include "d_heap.h"

void d_heap::Swap(int node1, int node2)
{
	int tmp = weight[node1];
	weight[node1] = weight[node2];
	weight[node2] = tmp;
}

void d_heap::Diving(int node)
{
	int curr_node = node;
	int child = GetMinChild(node);
	while (child != -1 && weight[curr_node] > weight[child])
	{
		Swap(curr_node, child);
		curr_node = child;
		child = GetMinChild(curr_node);
	}
}

void d_heap::Emersion(int node)
{
	int curr_node = node;
	int parent = GetParent(node);
	while (parent != -1 && weight[curr_node] < weight[parent])
	{
		Swap(curr_node, parent);
		curr_node = parent;
		parent = GetParent(curr_node);
	}
}

int d_heap::GetLeft(int node)
{
	int tmp = node*d + 1;
	if (tmp >= power)
		return -1;
	return tmp;
}

int d_heap::GetRight(int node)
{
	int tmp = GetLeft(node);
	if (tmp == -1)
		return -1;
	tmp += (d - 1);
	if (tmp >= power - 1)
		return power - 1;
	return tmp;
}

int d_heap::GetParent(int node)
{
	if (!node)
		return -1;
	return (node - 1) / d;
}

int d_heap::GetMinChild(int node)
{
	int child1 = GetLeft(node);
	if (child1 == -1)
		return -1;

	int min = weight[child1];
	int min_node = child1;

	for (int i = child1 + 1; i <= GetRight(node); i++)
	{
		if (min > weight[i])
		{
			min = weight[i];
			min_node = i;
		}
	}
	return min_node;
}

void d_heap::Insert(int node, int w)
{
	

}

void d_heap::DecreaseWeight(int node, int delta)
{
	weight[node] -= delta;
	if (delta > 0)
		Emersion(node);
	else
		Diving(node);
}

d_node* d_heap::DeleteMin()
{
	d_node *tmp = new d_node;
	//tmp->node=
	Swap(0, power - 1);
	power--;
	Diving(0);
}

d_node* d_heap::Delete(int node)
{
	DecreaseWeight(node, weight[node]);
	DecreaseWeight(node, INT_MAX);
	DeleteMin();
}

void d_heap::MakeHeap(int *w, int num)
{
	weight = new int[num];
	for (int i = 0; i < num; i++)
		weight[i] = w[i];
	power = num;

	for (int i = power - 1; i >= 0; i--)
		Diving(i);
}