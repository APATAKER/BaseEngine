#include "FSMSystem.h"

FSMSystem::FSMSystem(void)
	: mIsRunning(false)
	, mCurrentState(0)
{

}

FSMSystem::~FSMSystem(void)
{
	// TODO: Cleanup
}

//States our FSM and begins the first state entered in our vector
void FSMSystem::Start(void) {
	if (mStateVec.size() == 0) {
		printf("Finite State Machine is empty! Cannot start it!");
		return;
	}

	mIsRunning = true;
	TransitionToState(mStateVec[0]);
}

//adds state to our FSM that our AI agent can switch to
void FSMSystem::AddState(FSMState* state) {
	FSMState* key = state;
	if (find(mStateVec.begin(), mStateVec.end(), key) == mStateVec.end()) {
		mStateVec.push_back(state);
	}
}

//calls the Update function of our currently active state.
//checks every frame if a condition is met that triggers changing to a new state
//if triggered, than transition to the new state
void FSMSystem::Update(void)
{
	if (!mIsRunning) return;

	if (mCurrentState == 0) {
		printf("Error: The current state is null!");
		mIsRunning = false;
		return;
	}

	mCurrentState->Update();

	if (mCurrentState->IsDone()) {
		FSMState* newState = mCurrentState->GetTransition();
		TransitionToState(newState);

	}

}

//calls OnExitState() before switching to our new state
//we then have our new state call onEnterState()
void FSMSystem::TransitionToState(FSMState* state) {
	if (state == 0) {
		printf("Error: [FSMSystem::TransitionToState: The state is null");
		mIsRunning = false;
		return;
	}

	if (mCurrentState != 0) {
		mCurrentState->OnExitState();
	}

	mCurrentState = state;
	mCurrentState->OnEnterState();
}

//Resets our FSM
//we call OnExitState of any current state we have active before resetting
void FSMSystem::Reset(void) {
	if (mCurrentState != 0) {
		mCurrentState->OnExitState();
	}
	mCurrentState = 0;
	mIsRunning = false;
}