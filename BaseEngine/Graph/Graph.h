#pragma once

#include <vector>

struct Node
{
	char id;
	bool hasGoal;
	bool visited;
	float costsofar;
	struct Node* parent;
	std::vector<std::pair<Node*, float>> children;	//Edge<child node,edge weight>
};


class Graph
{
public:
	Graph();
	void CreateNode(char id, bool bHasGoal = false);
	void AddEdge(Node* parent, Node* child, float weight);
	void printGraph();
	void ResetGraph();
	std::vector<Node*> nodes;
};