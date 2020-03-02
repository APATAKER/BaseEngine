#include "cCoordinator.h"


physLib::cCoordinator::cCoordinator():
coordinator_position(glm::vec3(0,0,0))
{
	formation_type_ = physLib::formation_type::square;
	if(formation_type_ == physLib::formation_type::square)
	{
		position_offset_.push_back(glm::vec3(-5, 0, 0));		// 1
		position_offset_.push_back(glm::vec3(5, 0, 0));		// 2
		position_offset_.push_back(glm::vec3(-15, 0, 0));		// 3
		position_offset_.push_back(glm::vec3(15, 0, 0));		// 4
		position_offset_.push_back(glm::vec3(-5, 0, -10));		// 5
		position_offset_.push_back(glm::vec3(-5, 0, 10));		// 6
		position_offset_.push_back(glm::vec3(5, 0, -10));		// 7
		position_offset_.push_back(glm::vec3(5, 0, 10));		// 8
		position_offset_.push_back(glm::vec3(-15, 0, -10));		// 9
		position_offset_.push_back(glm::vec3(-15, 0, 10));		// 10
		position_offset_.push_back(glm::vec3(15, 0, -10));		// 11
		position_offset_.push_back(glm::vec3(15, 0, 10));		// 12
		
	}
	path_.push_back(glm::vec3(0, 0, 0));
	path_.push_back(glm::vec3(-170, 0, -170));
	path_.push_back(glm::vec3(-170, 0, 170));
	path_.push_back(glm::vec3(170, 0, 170));
	path_.push_back(glm::vec3(170, 0, -170));

	rev_path_.push_back(glm::vec3(0, 0, 0));
	rev_path_.push_back(glm::vec3(170, 0, -170));
	rev_path_.push_back(glm::vec3(170, 0, 170));
	rev_path_.push_back(glm::vec3(-170, 0, 170));
	rev_path_.push_back(glm::vec3(-170, 0, -170));
	
}
void physLib::cCoordinator::ai_update()
{
	for(int i=0;i<vehicles_.size();i++)
	{
		
	}
}

void physLib::cCoordinator::set_formation_type(const formation_type f_type)
{
	this->formation_type_ = f_type;
}



void physLib::cCoordinator::update_position_offset()
{
	
	
	if (formation_type_ == physLib::formation_type::square)
	{
		position_offset_[0] = (glm::vec3(-5, 0, 0));					// 1
		position_offset_[1] = (glm::vec3(5, 0, 0));						// 2
		position_offset_[2] = (glm::vec3(-15, 0, 0));					// 3
		position_offset_[3] = (glm::vec3(15, 0, 0));					// 4
		position_offset_[4] = (glm::vec3(-5, 0, -10));					// 5
		position_offset_[5] = (glm::vec3(-5, 0, 10));					// 6
		position_offset_[6] = (glm::vec3(5, 0, -10));					// 7
		position_offset_[7] = (glm::vec3(5, 0, 10));					// 8
		position_offset_[8] = (glm::vec3(-15, 0, -10));					// 9
		position_offset_[9] = (glm::vec3(-15, 0, 10));					// 10
		position_offset_[10] = (glm::vec3(15, 0, -10));					// 11
		position_offset_[11] = (glm::vec3(15, 0, 10));					// 12

	}
	else if (formation_type_ == physLib::formation_type::circle)
	{
		position_offset_[0] = (glm::vec3(0, 0, -30));					// 1
		position_offset_[1] = (glm::vec3(0, 0, 30));					// 2
		position_offset_[2] = (glm::vec3(-30, 0, 0));					// 3
		position_offset_[3] = (glm::vec3(30, 0, 0));					// 4
		position_offset_[4] = (glm::vec3(-13, 0, 27));					// 5
		position_offset_[5] = (glm::vec3(-13, 0, -27));					// 6
		position_offset_[6] = (glm::vec3(13, 0, 27));					// 7
		position_offset_[7] = (glm::vec3(13, 0, -27));					// 8
		position_offset_[8] = (glm::vec3(-27, 0, -13));					// 9
		position_offset_[9] = (glm::vec3(-27, 0, 13));					// 10
		position_offset_[10] = (glm::vec3(27, 0, -13));					// 11
		position_offset_[11] = (glm::vec3(27, 0, 13));					// 12


	}
	else if (formation_type_ == physLib::formation_type::line)
	{
		position_offset_[0] = (glm::vec3(0, 0, 0));			// 1
		position_offset_[1] = (glm::vec3(0, 0, -10));		// 2
		position_offset_[2] = (glm::vec3(0, 0, 10));		// 3
		position_offset_[3] = (glm::vec3(0, 0, -20));		// 4
		position_offset_[4] = (glm::vec3(0, 0, 20));		// 5
		position_offset_[5] = (glm::vec3(0, 0, -30));		// 6
		position_offset_[6] = (glm::vec3(0, 0, 30));		// 7
		position_offset_[7] = (glm::vec3(0, 0, -40));		// 8
		position_offset_[8] = (glm::vec3(0, 0, 40));		// 9
		position_offset_[9] = (glm::vec3(0, 0, -50));		// 10
		position_offset_[10] = (glm::vec3(0, 0, 50));		// 11
		position_offset_[11] = (glm::vec3(0, 0, -60));		// 12

	}
	else if (formation_type_ == physLib::formation_type::rows)
	{
		position_offset_[0] = (glm::vec3(0, 0, 0)) ;					// 1
		position_offset_[1] = (glm::vec3(-10, 0, 0));					// 2
		position_offset_[2] = (glm::vec3(10, 0, 0));					// 3
		position_offset_[3] = (glm::vec3(-20, 0, 0));					// 4
		position_offset_[4] = (glm::vec3(20, 0, 0));					// 5
		position_offset_[5] = (glm::vec3(30, 0, 0));					// 6
		position_offset_[6] = (glm::vec3(-10, 0, 10));					// 7
		position_offset_[7] = (glm::vec3(10, 0, 10));					// 8
		position_offset_[8] = (glm::vec3(-20, 0, 10));					// 9
		position_offset_[9] = (glm::vec3(20, 0, 10));					// 10
		position_offset_[10] = (glm::vec3(30, 0, 10));					// 11
		position_offset_[11] = (glm::vec3(0, 0, 10));					// 12

	}
	else if (formation_type_ == physLib::formation_type::v)
	{
		position_offset_[0] = (glm::vec3(-10, 0, 10));					// 1
		position_offset_[1] = (glm::vec3(10, 0, 10));					// 2
		position_offset_[2] = (glm::vec3(-20, 0, 20));					// 3
		position_offset_[3] = (glm::vec3(20, 0, 20));					// 4
		position_offset_[4] = (glm::vec3(-30, 0, 30));					// 5
		position_offset_[5] = (glm::vec3(30, 0, 30));					// 6
		position_offset_[6] = (glm::vec3(-40, 0, 40));					// 7
		position_offset_[7] = (glm::vec3(40, 0, 40));					// 8
		position_offset_[8] = (glm::vec3(-50, 0, 50));					// 9
		position_offset_[9] = (glm::vec3(50, 0, 50));					// 10
		position_offset_[10] = (glm::vec3(-60, 0, 60));					// 11
		position_offset_[11] = (glm::vec3(60, 0, 60));					// 12

	}
	glm::quat rotationadjust = coordinator->safeQuatLookAt(path_[current_path], coordinator->mPosition, glm::vec3(0, 1, 0));

	//glm::quat rotateThisFrame = glm::slerp(rotationadjust, dt);
	//coordinator->mOrientation = rotationadjust;

	for(int i=0;i<position_offset_.size();i++)
	{
		position_offset_[i] += coordinator->mPosition;
		//vehicles_[i]->mOrientation = coordinator->mOrientation;
	}
	
}

void physLib::cCoordinator::get_coordinator_at()
{
	coordinator->updateAtFromOrientation();
	coordinator->mOrientation = glm::quat(coordinator->getAtInWorldSpace());
}

void physLib::cCoordinator::position_offset_wrt_direction()
{
	switch (formation_type_)
	{
	case formation_type::square:
		
		break;
	case formation_type::circle:
		break;
	case formation_type::rows:
		break;
	case formation_type::line:
		break;
	case formation_type::v:
		break;
	}
}

