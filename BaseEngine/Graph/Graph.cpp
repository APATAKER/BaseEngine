#include "Graph.h"
#include <iostream>



Graph::Graph(int width, int height)
{
	graphWidth = width;
	graphHeight = height;
}

void Graph::CreateNode(char id)
{
	Node* node = new Node;

	node->id = id;
	if (node->id == '_')
		node->isTraversal = false;
	if (node->id == 'r' || node->id == 'b')
		node->hasGoal = true;
	if (node->id == 'r')
		node->hasResource = true;
	if (node->id == 'b')
		node->isDropPoint = true;
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

int Graph::GetGraphWidth()
{
	return graphWidth;
}

int Graph::GetGraphHeight()
{
	return graphHeight;
}

void Graph::printGraph()
{
	for(Node*& currNode :this->nodes)
	{
		std::cout << "Node: " << currNode->id << " -> ";
	}
}
void Graph::PrintParents(bool includeCost)
{
	for (Node*& currNode : this->nodes)
	{
		std::cout << "Node: " << currNode->id << " -> ";
		if (currNode->parent != NULL)
		{
			std::cout << currNode->parent->id;
		}
		else
		{
			std::cout << "NULL";
		}
		if (includeCost)
		{
			std::cout << " cost so far: " << currNode->costsofar << " hDist= " << currNode->hDistance << " f= " << currNode->costsofar + currNode->hDistance;
		}
		std::cout << std::endl;
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
