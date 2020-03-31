#ifndef IdleState_H
#define IdleState_H

#include "FSMState.h"

#include "MapInfo.h"

class IdleState : public FSMState {
public:
	IdleState(void);
	virtual ~IdleState(void);

	virtual void Update(cGameObject* cur_gameObject);

	virtual void OnEnterState(void);
	virtual void OnExitState(void);
	
	virtual Node* GetResNode(void);

};

#endif
