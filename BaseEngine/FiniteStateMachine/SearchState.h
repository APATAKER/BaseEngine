#ifndef SearchState_H
#define SearchState_H

#include "FSMState.h"

#include "MapInfo.h"

class SearchState : public FSMState {
public:
	SearchState(void);
	virtual ~SearchState(void);

	virtual void Update(void);

	virtual void OnEnterState(void);
	virtual void OnExitState(void);

};

#endif
