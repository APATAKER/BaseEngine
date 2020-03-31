#include "SearchState.h"

SearchState::SearchState(void)
	: FSMState("Search State") {
}

SearchState::~SearchState(void){
}

void SearchState::Update(void) {
	if (gNumResources == 0) {
		printf("SearchState: No resources found!\n");
		mCurrentCondition = 2;
	}
	else {
		printf("SearchState: Found %d resources!\n", gNumResources);
		mCurrentCondition = 1;
	}
}

void SearchState::OnEnterState(void) {
	printf("SearchState: Entered\n");
	mCurrentCondition = 0;
}

void SearchState::OnExitState(void) {
	printf("SearchState: Exited\n");
}
