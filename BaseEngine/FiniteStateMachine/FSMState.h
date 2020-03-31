#ifndef FSMState_H
#define FSMState_H

#include <string>
#include <map>

#include "../GameObject/cGameObject.h"
#include "../Graph/Graph.h"
//class cGameObject;
using namespace std;

class FSMState
{
public:
	FSMState(std::string name);
	virtual ~FSMState(void);

	//Action to take when current state is active
	virtual void Update(cGameObject* cur_gameObject) = 0;
	//Action to take when current state is being entered
	virtual void OnEnterState(void) = 0;
	//Action to take when current state is being exited from
	virtual void OnExitState(void) = 0;

	virtual Node* GetResNode(void) = 0;

	bool IsDone(void); //a condition has been met that triggers a transition

	void linkState(FSMState* state);

	void AddTransition(int condition, FSMState* state); //add a new transition to mTransitionMap, includes condition as map key
	FSMState* GetTransition(); //returns state that we want to transition to
protected:
	FSMState(void);
	int mCurrentCondition; //0 = state has not performed action, 1 = state has perfomred action, is now ready to switch states
	FSMState* mLinkedState;

private:
	string mStateName;
	map<int, FSMState*> mTransitionMap; //holds all the states our current state can transition to


};

#endif
