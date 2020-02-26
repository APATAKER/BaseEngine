#include "cCoordinator.h"

physLib::cCoordinator::cCoordinator()
{
	formation_type_ = physLib::formation_type::square;
	position_offset_[0] = glm::vec3(2, 0, -2);
	
	
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
