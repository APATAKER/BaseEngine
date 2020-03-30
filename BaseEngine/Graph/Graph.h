#pragma once

#include <vector>

struct Node
{
	char id;
	std::pair<int, int> nodePoint;
	bool hasGoal = false;
	bool isTraversal = true;
	bool hasTerrain = false;
	bool visited;
	float costsofar;
	float hDistance;
	struct Node* parent;
	std::vector<std::pair<Node*, float>> children;	//Edge<child node,edge weight>
};


class Graph
{
public:
	Graph();
	void CreateNode(char id);
	void AddEdge(Node* parent, Node* child, float weight);
	void printGraph();
	void ResetGraph();
	std::vector<Node*> nodes;
};