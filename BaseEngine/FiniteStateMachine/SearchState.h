#ifndef SearchState_H
#define SearchState_H

#include "FSMState.h"
#include "../Graph/Graph.h"

#include "MapInfo.h"

class SearchState : public FSMState {
	bool is_found = false;
	bool is_ended = false;
	bool is_first = true;
	float slowingRadius = 10.f;
	float maxVelocity = 10.f;

	int start_res;
	std::vector<glm::vec3> vec_positions_to_reach_resource;
	Graph* graph;
	Node* start_node;
	Node* home_node;

	Node* res_node;
	
public:
	SearchState(std::vector<glm::vec3> vec_pos, Graph* graph);
	virtual ~SearchState(void);

	virtual void Update(cGameObject* cur_gameObjects);

	virtual void OnEnterState(void);
	virtual void OnExitState(void);

	virtual Node* GetResNode(void);

	glm::vec3 seek(glm::vec3 targetPos, cGameObject* gameobj, bool& is_ended);

};

#endif
