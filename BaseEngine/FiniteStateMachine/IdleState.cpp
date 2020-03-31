#include "IdleState.h"

IdleState::IdleState(void)
	: FSMState("Idle State") {
}

IdleState::~IdleState(void) {
}

void IdleState::Update(void) {
	mCurrentCondition = 1;
}

void IdleState::OnEnterState(void) {
	printf("IdleState: Entered\n");
	mCurrentCondition = 0;
}

void IdleState::OnExitState(void) {
	printf("IdleState: Exited\n");
}
