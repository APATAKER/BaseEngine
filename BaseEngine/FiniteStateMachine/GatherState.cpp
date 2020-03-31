#include "GatherState.h"

GatherState::GatherState(void)
	: FSMState("Gather State") {
}

GatherState::~GatherState(void) {
}

void GatherState::Update(void) {
	printf("GatherState: Gathered a resource!\n");
	gNumResources--;
	mCurrentCondition = 1;
}

void GatherState::OnEnterState(void) {
	printf("GatherState: Entered\n");
	mCurrentCondition = 0;
}

void GatherState::OnExitState(void) {
	printf("GatherState: Exited\n");
}
