#pragma once

#include <glm/vec3.hpp>
#include <vector>


struct Node
{
	char id;
	std::pair<int, int> nodePoint;
	bool hasGoal = false;
	bool hasResource = false;
	bool isDropPoint = false;
	bool isTraversal = true;
	bool hasTerrain = false;
	bool visited;
	float costsofar;
	float hDistance;
	glm::vec3 position;
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
	void PrintParents(bool includeCost);
	std::vector<Node*> nodes;
};

Node* Dijkstra(Node* rootNode, Graph* graph);
Node* AStar(Node* rootNode, Graph* graph, Node* goal);
int TotalSteps(Node* end_node);