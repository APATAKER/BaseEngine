#ifndef ReturnState_H
#define ReturnState_H

#include "FSMState.h"

#include <time.h>

#include "../Graph/Graph.h"
#include "MapInfo.h"

class ReturnState : public FSMState {
	clock_t start_time;
	clock_t time_elapsed;

	bool is_reached = false;
	bool is_ended = false;
	float slowingRadius = 10.f;
	float maxVelocity = 10.f;

	
	int start_home;
	std::vector<glm::vec3> vec_positions_to_reach_home;
	Graph* graph;
	Node* start_node;
	Node* home_node;


public:
	ReturnState(std::vector<glm::vec3> vec_pos, Graph* graph);
	virtual ~ReturnState(void);

	virtual void Update(cGameObject* cur_gameObject);

	virtual void OnEnterState(void);
	virtual void OnExitState(void);

	virtual Node* GetResNode(void);

	glm::vec3 seek(glm::vec3 targetPos, cGameObject* gameobj, bool& is_ended);

};

#endif
