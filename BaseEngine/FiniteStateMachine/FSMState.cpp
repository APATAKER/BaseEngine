#include "FSMState.h"
//FSMState file acts as the base class that handles the actions of each state.
//Typically in video games we want these three actions: OnEnterState, Update, and OnExitState
//FSMState also handles all the different transitions a state to go towards and also tell FSMSystem when a state is done

//Constructors
FSMState::FSMState(void)
	: mCurrentCondition(0) {
}

FSMState::FSMState(std::string name)
	: mStateName(name)
	, mCurrentCondition(0) {
}

FSMState::~FSMState(void) {
	// TODO: Cleanup
}

//Transitions
//This is where we include what states we are able to transition to when our current state is done functioning.
//Stores the different states that are reachable from our current state into a map
//Each new state includes a condition key which in a game could be triggered using ether internal logic or external logic
void FSMState::AddTransition(int condition, FSMState* state) {
	std::map<int, FSMState*>::iterator it = mTransitionMap.find(condition);
	if (it != mTransitionMap.end()) {
		printf("FSMState::AddTransition: State already has this condition\n");
		return;
	}
	mTransitionMap[condition] = state;
}

//returns the state we want to transition to
FSMState* FSMState::GetTransition() {
	if (!IsDone()) return 0;
	return mTransitionMap[mCurrentCondition];
}

//used to check if we are done with our state and want to transition to a new state
bool FSMState::IsDone(void) {
	return mCurrentCondition != 0;
}

//Actions
//runs the internal logic of your AI. You can include just one action or a list of actions you want your state machine to loop through.
//These actions are just virtual functions that will be overridden by the functions in the specific state classes (IdleState.h, SearchState.h, etc)
void FSMState::Update()
{

}

//handle values we need to initialize when entering any state.
//used to set up variables, create objects, initialization, etc
//anything we need to set up before switching to our states logic
//Example: Your barbarian NPC wants to attack someone so it runs a "draw weapon" animation when switching to an attack state. 
void FSMState::OnEnterState() {
	printf("Entering %s state... \n", mStateName.c_str());
}

//handles anything we need to close or conclude with our current state. 
//This function is called before the OnEnterState() function of the new state we are visiting next.
//Example: Your barbarian warrior NPC has his axe out and needs run the "sheath" animation since its no longer attacking 
//before exiting attack mode and switching to a climb state to move up a cliff
void FSMState::OnExitState() {
	printf("Exiting %s state... \n", mStateName.c_str());
}


