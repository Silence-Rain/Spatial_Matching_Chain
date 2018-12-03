#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <map>
#include <vector>
#include <deque>
#include "Node.h"
#include "Pair.h"
#include "kdtree.h"

using namespace std;

void readFile(string, map<string, Node>&, map<string, Node>&);
void writeFile(string, vector<Pair>);
string findNearestNeighbor(map<string, Node>, Node);

int main()
{
	map<string, Node> P;			// Service providers
	map<string, Node> O;			// Users
	deque<Node*> C;					// Chain
	vector<Pair> A;					// Match

	

	readFile("./data/input.dat", P, O);

	srand(time(0));

	//// 生成 O 的所有 keys
	//vector<string> OKeys;
	//for (map<string, Node>::iterator it = O.begin(); it != O.end(); ++it)
	//{
	//	OKeys.push_back(it->first);
	//}

	while (!O.empty())
	{
		// pick the first object o from O
		// random? O(n^2)
		// Node* o = &O[OKeys[rand() % O.size()]];
		map<string, Node>::iterator it = O.begin();
		Node* o = &(it->second);

		// insert {o} into C
		C.push_front(o);

		while (!C.empty() && !O.empty())
		{
			Node* x = C.front();

			if (O.find(x->id) != O.end())
			{
				string y_key = findNearestNeighbor(P, *x);
				Node* y = &P[y_key];

				if (C.size() >= 2 && y == C[1])
				{
					if (x->weight > y->weight)
					{
						Pair pair(*y, *x, y->weight);
						A.push_back(pair);
						x->weight -= y->weight;
						// remove y from P
						P.erase(y->id);
						// remove x and y from C
						C.pop_front(); C.pop_front();
					}
					else
					{
						Pair pair(*y, *x, x->weight);
						A.push_back(pair);
						y->weight -= x->weight;
						// remove x from O and C
						C.pop_front();
						O.erase(x->id);

						if (y->weight == 0)
						{
							// remove y from P and C 
							C.pop_front();
							P.erase(y->id);
						}
					}
				}
				else
				{
					C.push_front(y);
				}
			}
			else
			{
				string y_key = findNearestNeighbor(O, *x);
				Node* y = &O[y_key];

				if (C.size() >= 2 && y == C[1])
				{
					if (y->weight > x->weight)
					{
						Pair pair(*x, *y, x->weight);
						A.push_back(pair);
						y->weight -= x->weight;
						// remove x from P
						P.erase(x->id);
						// remove x and y from C
						C.pop_front(); C.pop_front();
					}
					else
					{
						Pair pair(*x, *y, y->weight);
						A.push_back(pair);
						x->weight -= y->weight;
						// remove y from O and C
						C.erase(C.begin() + 1);
						O.erase(y->id);

						if (x->weight == 0)
						{
							// remove x from P and C 
							C.pop_front();
							P.erase(x->id);
						}
					}
				}
				else
				{
					C.push_front(y);
				}
			}
		}
	}
	
	writeFile("./data/output.dat", A);

	return 0;
}

void readFile(string path, map<string, Node>& p, map<string, Node>& o)
{
	ifstream inFile(path);

	if (!inFile)
	{
		cerr << "Cannot open input file!" << endl;
		exit(1);
	}

	string id;
	int x, y, w;

	while (inFile >> id >> x >> y >> w)
	{
		Node temp(id, x, y, w);
		if (id[0] == 'p')
		{
			p.insert(pair<string, Node>(id, temp));
		}
		else
		{
			o.insert(pair<string, Node>(id, temp));
		}
	}
}

void writeFile(string path, vector<Pair> v)
{
	ofstream outFile(path);

	if (!outFile)
	{
		cerr << "Cannot open output file!" << endl;
		exit(1);
	}

	for (int i = 0; i < v.size(); i++)
	{
		outFile << "(" << v[i].node1.id << ", " << v[i].node2.id << ", " << v[i].weight << ")" << endl;
	}
}

string findNearestNeighbor(map<string, Node> m, Node n)
{
	KDTree* kd = new KDTree();
	const int size = m.size();
	Node* exm_set = new Node[size];  
	int i = 0;

	for (map<string, Node>::iterator it = m.begin(); it != m.end(); ++it)
	{
		exm_set[i] = it->second;
		i++;
	}

	struct Tnode * root = NULL;
	root = kd->build_kdtree(exm_set, size, root);

	delete[] exm_set;

	Node nearestpoint;
	double distance;

	return kd->searchNearest(root, n, nearestpoint, distance);
}