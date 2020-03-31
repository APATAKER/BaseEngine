#ifndef GatherState_H
#define GatherState_H

#include <time.h>

#include "FSMState.h"

#include "MapInfo.h"

class GatherState : public FSMState {

	clock_t start_time;
	clock_t time_elapsed;
public:
	GatherState(void);
	virtual ~GatherState(void);

	virtual void Update(cGameObject* cur_gameObject);

	virtual void OnEnterState(void);
	virtual void OnExitState(void);
	virtual Node* GetResNode(void);
};

#endif
