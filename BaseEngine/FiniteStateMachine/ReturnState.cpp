#include "ReturnState.h"

ReturnState::ReturnState(void)
	: FSMState("Return State") {
}

ReturnState::~ReturnState(void) {
}

void ReturnState::Update(void) {
	printf("ReturnState: Returned a resource!\n");
	mCurrentCondition = 1;
}

void ReturnState::OnEnterState(void) {
	printf("ReturnState: Entered\n");
	mCurrentCondition = 0;
}

void ReturnState::OnExitState(void) {
	printf("ReturnState: Exited\n");
}
