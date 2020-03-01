#pragma once
#include <glm/vec3.hpp>
#include <vector>
#include "cRigidBody.h"

namespace physLib
{

	enum class formation_type {
		circle,
		v,
		square,
		line,
		rows
	};
	
	class cCoordinator
	{
		friend  class cAI;
		friend class cWorld;
		
		cCoordinator();
		glm::vec3 coordinator_position;
		glm::vec3 steering_force;
		glm::vec3 velocity;
		std::vector< glm::vec3> position_offset_;
		std::vector<glm::vec3> path_;
		std::vector<glm::vec3> rev_path_;
		std::vector<cRigidBody*> vehicles_;
		cRigidBody* coordinator;
		formation_type formation_type_;
	public:
		void ai_update();
		void set_formation_type(formation_type f_type);
		void set_coordinator();
		void update_position_offset();
		
		
	};
	
}