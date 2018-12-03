#pragma once
#include "Position.h"
#include <string>

using namespace std;

struct Node
{
	string id;
	int weight;
	Position pos;

	Node(string id = "", int x = 0, int y = 0, int w = 0)
		:pos(x, y)
	{
		this->id = id;
		this->weight = w;
	}

	Node(const Node& n)
		:pos(n.pos)
	{
		this->id = n.id;
		this->weight = n.weight;
	}

	bool operator==(const Node& n)
	{
		return this->id == n.id;
	}
};