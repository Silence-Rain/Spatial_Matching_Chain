#pragma once
#include "Node.h"

struct Pair
{
	Node node1;
	Node node2;
	int weight;

	Pair(Node n1, Node n2, int w)
		:node1(n1), node2(n2)
	{
		this->weight = w;
	}
};