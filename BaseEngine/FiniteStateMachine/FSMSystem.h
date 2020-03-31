#ifndef FSMSystem_H
#define FSMSystem_H

#include <vector>
#include <string>

#include "FSMState.h"

class FSMSystem
{
public:
	FSMSystem(void);
	virtual ~FSMSystem(void);

	void AddState(FSMState* state);
	void TransitionToState(FSMState* state);

	void Update(void);

	void Start(void);
	void Reset(void);

private:
	vector<FSMState*> mStateVec; //keeps track of all the states our FSM can enter
	bool mIsRunning; //is our FSM currently active

	FSMState* mCurrentState;
};

#endif
