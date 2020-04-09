#include "ReturnState.h"


ReturnState::ReturnState(std::vector<glm::vec3> vec_pos, Graph* graph)
	: FSMState("Return State") {
	vec_positions_to_reach_home = vec_pos;
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

ReturnState::~ReturnState(void) {
}

void ReturnState::Update(cGameObject* cur_gameObject) {

	if(is_reached)
	{
		
		time_elapsed = (clock() - start_time) / CLOCKS_PER_SEC;
		if (time_elapsed >= 2)
		{
			printf("ReturnState: Waited 2 Sec and Returned a resource!\n");
			mCurrentCondition = 1;
		}
	}
	else
	{
		mCurrentCondition = 0;
		if (!is_ended)
		{
			cur_gameObject->m_velocity = seek(vec_positions_to_reach_home[start_home], cur_gameObject, is_ended);

		}
		else
		{
			if (start_home > 0)
			{
				start_home--;
				is_ended = false;
				return;
			}


			cur_gameObject->m_velocity = glm::vec3(0);
			cur_gameObject->m_physics_component->SetPosition(vec_positions_to_reach_home[0]);
			is_reached = true;
			printf("ReturnState: Resource being Deposited!\n");
			start_time = clock();


		}

	}
}

void ReturnState::OnEnterState(void) {
	printf("ReturnState: Entered\n");
	mCurrentCondition = 0;
	Node* res_node = mLinkedState->GetResNode();
	
	Node* returned_node = AStar(res_node, graph, home_node);
	int total_steps_needed_to_reach_home_base = TotalSteps(returned_node);

	vec_positions_to_reach_home.clear();
	Node* temp_node2 = returned_node;
	for (int index = 0; index < total_steps_needed_to_reach_home_base; index++)
	{
		vec_positions_to_reach_home.push_back(temp_node2->position);
		temp_node2 = temp_node2->parent;
	}

	
	start_home = vec_positions_to_reach_home.size() - 1;
	printf("ReturnState: Returning To HomeBase\n");

}

void ReturnState::OnExitState(void) {
	printf("ReturnState: Exited\n");
	is_reached = false;
	is_ended = false;
}

Node* ReturnState::GetResNode(void)
{
	return nullptr;
}

glm::vec3 ReturnState::seek(glm::vec3 targetPos, cGameObject* gameobj, bool& is_ended)
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
