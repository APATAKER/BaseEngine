#include "GatherState.h"

#include <iostream>

GatherState::GatherState(void)
	: FSMState("Gather State") {
}

GatherState::~GatherState(void) {
}

void GatherState::Update(cGameObject* cur_gameObject) {
	time_elapsed = (clock() - start_time) / CLOCKS_PER_SEC;
	if(time_elapsed >= 7)
	{
		std::cout << "GatherState: 7Secs Passed And Resource Gathered" << std::endl;
		gNumResources--;
		mCurrentCondition = 1;
	}
}

void GatherState::OnEnterState(void) {
	printf("GatherState: Entered\n");
	mCurrentCondition = 0;
	printf("GatherState: Gathering a resource!\n");
	start_time = clock();

}

void GatherState::OnExitState(void) {
	printf("GatherState: Exited\n");
}

Node* GatherState::GetResNode(void)
{
	return nullptr;
}
