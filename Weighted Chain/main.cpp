#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cmath>
#include <map>
#include <vector>
#include <deque>
#include "Node.h"
#include "Pair.h"
#include "Chain.h"

using namespace std;

void readFile(string, map<string, Node>&, map<string, Node>&);
void writeFile(string, vector<Pair>);
void log(string, string);

int main(int argc, char** argv)
{
	map<string, Node> P;			// Service providers
	map<string, Node> O;			// Users
	deque<Node*> C;					// Chain
	vector<Pair> A;					// Match
	string inputFile;
	clock_t startTime, finishTime, preTime = 0, exeTime = 0, timeCost;

	if (argc > 1)
	{
		inputFile = string(argv[1]);
	}
	else
	{
		inputFile = "./data/input.dat";
	}

	log("info", "Read dataset: Begin...");
	readFile(inputFile, P, O);
	log("info", "Read dataset: Success!");
	log("info", "--------------------------------");

	log("info", "Use Weighted Chain to find best matching...");
	startTime = clock();

	weightedChain(P, O, C, A, preTime, exeTime);

	finishTime = clock();
	timeCost = 1.0 * (finishTime - startTime) / CLOCKS_PER_SEC;
	log("info", "Weighted Chain complete!");
	log("info", "Total time: " + to_string(timeCost));
	log("info", "Preprocessing time: " + to_string(preTime) + ", " + to_string(round(preTime / (timeCost == 0 ? 1 : timeCost))) + "%");
	log("info", "Execution time: " + to_string(exeTime) + ", " + to_string(round(exeTime / (timeCost == 0 ? 1 : timeCost))) + "%");

	log("info", "--------------------------------");
	log("info", "Write result: Begin...");
	writeFile("./data/output.dat", A);
	log("info", "Write result: Success!");

	return 0;
}

void readFile(string path, map<string, Node>& p, map<string, Node>& o)
{
	ifstream inFile(path);

	if (!inFile)
	{
		log("error", "Cannot open file: " + path);
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
		log("error", "Cannot open file: " + path);
		exit(1);
	}

	for (int i = 0; i < v.size(); i++)
	{
		outFile << "(" << v[i].node1.id << ", " << v[i].node2.id << ", " << v[i].weight << ")" << endl;
	}
}

void log(string type, string content)
{
	time_t now = time(0);
	char dateStr[26];
	ctime_s(dateStr, 26, &now);
	string date(dateStr);
	cout << "[" << date.substr(0, date.size() - 1) << "] " << "[" << type << "]: " << content << endl;
}