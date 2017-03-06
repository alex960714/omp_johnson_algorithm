#ifndef _D_HEAP_H_
#define _D_HEAP_H_

#include <map>
using namespace std;

struct d_node 
{
	int node;
	int weight;
	/*d_node *prev;
	d_node **child;*/
};

class d_heap
{
private:
	int d;
	int power;
	d_node *root;
	int *weight;

	void Swap(int node1, int node2);
	void Diving(int node);
	void Emersion(int node);
public:
	d_heap(int deg = 2) { d = deg; };
	~d_heap() {};

	int GetLeft(int node);
	int GetRight(int node);
	int GetParent(int node);
	int GetMinChild(int node);

	void Insert(int node, int w);
	void DecreaseWeight(int node, int delta);
	d_node* DeleteMin();
	d_node* Delete(int node);
	void MakeHeap(int *w, int num);

	bool IsEmpty() { return !power; }
};
#endif
