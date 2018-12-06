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

void readFile(string, map<string, Node>&, string);
void writeFile(string, vector<Pair>);
void log(string, string);

int main(int argc, char** argv)
{
	map<string, Node> P;			// Service providers
	map<string, Node> O;			// Users
	deque<Node*> C;					// Chain
	vector<Pair> A;					// Match
	string pFile, oFile;
	clock_t startTime, finishTime, preTime = 0, exeTime = 0, timeCost;

	if (argc == 3)
	{
		pFile = string(argv[1]);
		oFile = string(argv[2]);
	}
	else
	{
		pFile = "./data/PData_1";
		oFile = "./data/OData_1";
	}

	log("info", "Read dataset " + pFile + ", " + oFile + ": Begin...");
	readFile(pFile, P, "p");
	readFile(oFile, O, "o");

	log("info", "Read dataset " + pFile + ", " + oFile + ": Success!");
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

	// log("info", "--------------------------------");
	// log("info", "Write result: Begin...");
	// writeFile("../data/output.dat", A);
	// log("info", "Write result: Success!");

	return 0;
}

void readFile(string path, map<string, Node>& data, string label)
{
	ifstream inFile(path);

	if (!inFile)
	{
		log("error", "Cannot open file: " + path);
		exit(1);
	}

	int x, y, w, i = 0;

	while (inFile >> w >> x >> y)
	{
		string id = label + to_string(i);
		Node temp(id, x, y, w);
		data.insert(pair<string, Node>(id, temp));
		i++;
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
	char* dateStr = ctime(&now);
	string date(dateStr);
	cout << "[" << date.substr(0, date.size() - 1) << "] " << "[" << type << "]: " << content << endl;
}