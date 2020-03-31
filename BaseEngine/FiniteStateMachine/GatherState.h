#ifndef GatherState_H
#define GatherState_H

#include "FSMState.h"

#include "MapInfo.h"

class GatherState : public FSMState {
public:
	GatherState(void);
	virtual ~GatherState(void);

	virtual void Update(void);

	virtual void OnEnterState(void);
	virtual void OnExitState(void);
};

#endif
