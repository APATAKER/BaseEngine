#include "SearchState.h"



SearchState::SearchState(std::vector<glm::vec3> vec_pos, Graph* graph)
	: FSMState("Search State") {
	vec_positions_to_reach_resource = vec_pos;
	start_node = new Node();
	home_node = new Node();
	this->graph = graph;
	int point = 0;
	for (int x = 0, draw1 = 0; x < graph->GetGraphWidth(); x++, draw1 += 1) {
		for (int y = 0, draw2 = 0; y < graph->GetGraphHeight(); y++, draw2 += 1, point++) {
			char check = graph->nodes[point]->id;
			if (check == 'g')
			{
				start_node = graph->nodes[point];
			}
			if (check == 'b')
			{
				home_node = graph->nodes[point];
			}
		}
	}

}

SearchState::~SearchState(void){
}

void SearchState::Update(cGameObject* cur_gameObjects) {
	if (gNumResources == 0) {
		printf("SearchState: No resources found!\n");
		mCurrentCondition = 2;
		return;
	}

	if(is_found == true)
	{
		printf("SearchState: Found %d resources!\n", gNumResources);
		mCurrentCondition = 1;
		
	}
	else 
	{
		mCurrentCondition = 0;
		if(!is_ended)
		{
			cur_gameObjects->m_velocity = seek(vec_positions_to_reach_resource[start_res], cur_gameObjects, is_ended);
			
		}
		else
		{
			if (start_res > 0)
			{
				start_res--;
				is_ended = false;
				return;
			}

			
			cur_gameObjects->m_velocity = glm::vec3(0);
			cur_gameObjects->m_physics_component->SetPosition(vec_positions_to_reach_resource[0]);
			is_found = true;

		}
	}
}

void SearchState::OnEnterState(void) {
	printf("SearchState: Entered\n");
	mCurrentCondition = 0;
	
//Node* res_node;
	if(is_first)
	{
		res_node = Dijkstra(start_node, graph);
		is_first = false;
	}
	else
	{
		res_node = Dijkstra(home_node, graph);
		
	}
	if (gNumResources > 0)
	{
		int total_steps_needed_to_reach_resource = TotalSteps(res_node);
		vec_positions_to_reach_resource.clear();
		Node* temp_node = res_node;
		for (int index = 0; index < total_steps_needed_to_reach_resource; index++)
		{
			vec_positions_to_reach_resource.push_back(temp_node->position);
			temp_node = temp_node->parent;
		}

		start_res = vec_positions_to_reach_resource.size() - 1;
		printf("SearchState: Searching\n");

	}
}

void SearchState::OnExitState(void) {
	printf("SearchState: Exited\n");
	is_found = false;
	is_ended = false;

	
}

Node* SearchState::GetResNode(void)
{
	return res_node;
}


glm::vec3 SearchState::seek(glm::vec3 targetPos, cGameObject* gameobj, bool& is_ended)
{
	glm::vec3 desieredVelocity = targetPos - gameobj->m_physics_component->GetPosition();

	float dist = desieredVelocity.length();

	glm::vec3 direction = glm::normalize(desieredVelocity);

	if (dist < slowingRadius)
	{
		desieredVelocity = direction * maxVelocity * (dist / slowingRadius);
	}
	else
	{
		desieredVelocity = direction * maxVelocity;
	}
	glm::vec3 steer = desieredVelocity - gameobj->m_velocity;
	float stop_distance = glm::distance(targetPos, gameobj->m_physics_component->GetPosition());
	if (stop_distance < 1.5f)
		is_ended = true;

	return steer;

}
