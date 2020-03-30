#include "Graph.h"
#include <iostream>


Graph::Graph(){}

void Graph::CreateNode(char id)
{
	Node* node = new Node;

	node->id = id;
	if (node->id == '_')
		node->isTraversal = false;
	if (node->id == 'r' || node->id == 'b')
		node->hasGoal = true;
	if (node->id == 'x')
		node->hasTerrain = true;
	node->costsofar = FLT_MAX;
	node->parent = NULL;
	node->hDistance = 0;
	node->visited = false; 

	this->nodes.push_back(node);
}

void Graph::AddEdge(Node* parent, Node* child, float weight)
{
	std::pair<Node*, float> edge;
	edge.first = child;
	edge.second = weight;
	parent->children.push_back(edge);
}

void Graph::printGraph()
{
	for(Node*& currNode :this->nodes)
	{
		std::cout << "Node: " << currNode->id << " -> ";
	}
}

void Graph::ResetGraph()
{
	for (Node*& currNode : this->nodes)
	{
		currNode->visited = false;
		currNode->parent = NULL;
		currNode->costsofar = FLT_MAX;
	}
}
