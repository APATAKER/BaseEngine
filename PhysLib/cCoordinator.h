#pragma once
#include <glm/vec3.hpp>
#include <vector>
#include "cRigidBody.h"

namespace physLib
{

	enum class formation_type {
		circle,
		square,
		v,
		line,
		rows
	};
	
	class cCoordinator
	{
		cCoordinator();
		std::vector< glm::vec3> position_offset_;
		std::vector<cRigidBody> vehicles_;
		formation_type formation_type_;
	public:
		void ai_update();
		void set_formation_type(formation_type f_type);
		
	};
	
}