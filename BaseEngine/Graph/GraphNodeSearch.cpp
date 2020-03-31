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

//Node* Dijikstra(Node* rootnode,Graph* graph)
//{
//	graph->ResetGraph();
//	for(int i =0;i<graph->nodes.size();i++)
//	{
//		graph->nodes[i]->costsofar = FLT_MAX;
//	}
//
//	rootnode->visited = true;
//	rootnode->costsofar = 0;
//	std::vector<Node*> closedList;
//	std::vector<Node*> openList;
//	openList.push_back(rootnode);
//
//	while (!openList.empty())
//	{
//		float mincost = FLT_MAX;
//		int minIndex = 0;
//		Node* currNode;
//
//
//		// remove
//		currNode = openList[minIndex];
//		
//	}
//	return rootnode; // placeholder return
//	
//}
bool IsNodeInOpenList(std::vector<Node*> openList, Node* child)
{
    for (int i = 0; i < openList.size(); i++)
    {
        if (openList[i] == child)
            return true;

    }
    return false;
}
int TotalSteps(Node* end_node)
{
    int count = 0;
    Node* temp = end_node;
    while(temp->parent)
    {
        count++;
        temp = temp->parent;
    }
    return count;
}

Node* Dijkstra(Node* rootNode, Graph* graph)
{
    graph->ResetGraph();

    rootNode->visited = true;
    rootNode->costsofar = 0;
    std::vector<Node*> closedList;
    std::vector<Node*> openList;
    openList.push_back(rootNode);

    while (!openList.empty())
    {
        float minCost = FLT_MAX;
        int minIndex = 0;
        Node* currNode;
        //find node with the lowest cost from root node
        for (size_t i = 0; i < openList.size(); i++)
        {
            if (openList[i]->costsofar < minCost)
            {
                minCost = openList[i]->costsofar;
                minIndex = i;
            }
        }

        //remove current node from open list and add to closed list
        currNode = openList[minIndex];
        for (auto iter = openList.begin(); iter != openList.end(); ++iter)
        {
            if (*iter == currNode)
            {
                openList.erase(iter);
                break;
            }
        }
        closedList.push_back(currNode);

       // std::cout << currNode->id << std::endl;
        currNode->visited = true;
        if (currNode->hasResource)
        {
            currNode->hasResource = false;
            return currNode;
        }

        //Go through every child node node 
        for (std::pair <Node*, float> child : currNode->children)
        {
            if (child.first->visited == false)
            {
                float weightSoFar = currNode->costsofar + child.second;
                if (weightSoFar < child.first->costsofar)
                {
                    //update node when new better path is found
                    child.first->costsofar = weightSoFar;
                    child.first->parent = currNode;
                    if (!IsNodeInOpenList(openList, child.first))
                    {
                        openList.push_back(child.first); //add newly discovered node to open list
                    }
                }
            }
        }
        //graph->PrintParents(true);
    }

    return NULL;
}
float CalculateHeuristics(Node* node, Node* goal)
{
    float D = 1;
    float dx = abs(node->position.x - goal->position.x);
    float dz = abs(node->position.z - goal->position.z);
    return D * (dx + dz);
}

Node* AStar(Node* rootNode, Graph* graph, Node* goal)
{
    graph->ResetGraph();

    rootNode->costsofar = 0;
    rootNode->hDistance = CalculateHeuristics(rootNode, goal);

    std::vector<Node*> closedList;
    std::vector<Node*> openList;
    openList.push_back(rootNode);

    while (!openList.empty())
    {
        float minCost = FLT_MAX;
        int minIndex = 0;
        Node* currNode;
        //find node with the lowest cost from root node and heuristic distance from the goal node
        for (size_t i = 0; i < openList.size(); i++)
        {
            if (openList[i]->costsofar + openList[i]->hDistance < minCost)
            {
                minCost = openList[i]->costsofar + openList[i]->hDistance;
                minIndex = i;
            }
        }

        //remove current node from open list and add to closed list
        currNode = openList[minIndex];
        for (auto iter = openList.begin(); iter != openList.end(); ++iter)
        {
            if (*iter == currNode)
            {
                openList.erase(iter);
                break;
            }
        }
        closedList.push_back(currNode);

        //std::cout << currNode->id << std::endl;
        currNode->visited = true;
        if (currNode->isDropPoint)
        {
            return currNode;
        }

        //Go through every child node node 
        for (std::pair <Node*, float> child : currNode->children)
        {
            if (child.first->visited == false)
            {
                float weightSoFar = currNode->costsofar + child.second;
                if (weightSoFar < child.first->costsofar)
                {
                    child.first->costsofar = weightSoFar;
                    child.first->parent = currNode;
                    if (!IsNodeInOpenList(openList, child.first))
                    {
                        child.first->hDistance = CalculateHeuristics(child.first, goal);
                        openList.push_back(child.first);
                    }
                }
            }
        }
        //graph->PrintParents(true);
    }

    return NULL;
}


