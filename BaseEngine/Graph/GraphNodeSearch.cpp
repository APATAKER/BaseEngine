#include <iostream>

#include "Graph.h"

#include <stack>
#include <queue>

Node* BFS(Node* rootNode,Graph* graph)
{
	rootNode->parent = NULL;
	rootNode->visited = true;
	std::queue<Node*> open_queue;
	open_queue.push(rootNode);

	while (!open_queue.empty())
	{
		Node* curr_node = open_queue.front();
		open_queue.pop();
		if(curr_node->hasGoal)
		{
			return curr_node;
		}
		std::cout << curr_node->id << std::endl;

		for(std::pair<Node*,float>& child:curr_node->children)
		{
			if(child.first->visited==false)
			{
				child.first->visited = true;
				child.first->parent = curr_node;
				open_queue.push(child.first);
			}
		}
				
	}
}

Node* Dijikstra(Node* rootnode,Graph* graph)
{
	graph->ResetGraph();
	for(int i =0;i<graph->nodes.size();i++)
	{
		graph->nodes[i]->costsofar = FLT_MAX;
	}

	rootnode->visited = true;
	rootnode->costsofar = 0;
	std::vector<Node*> closedList;
	std::vector<Node*> openList;
	openList.push_back(rootnode);

	while (!openList.empty())
	{
		float mincost = FLT_MAX;
		int minIndex = 0;
		Node* currNode;


		// remove
		currNode = openList[minIndex];
		
	}
	return rootnode; // placeholder return
	
}