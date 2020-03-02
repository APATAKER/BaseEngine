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
		int current_path = 0;
		glm::vec3 coordinator_position;
		glm::vec3 steering_force;
		glm::vec3 velocity;
		std::vector< glm::vec3> position_offset_;
		std::vector<glm::vec3> path_;
		std::vector<glm::vec3> rev_path_;
		std::vector<cRigidBody*> vehicles_;
		cRigidBody* coordinator;
		formation_type formation_type_;

		glm::vec3 direction_coord_to_path_destination;
	public:
		void ai_update();
		void set_formation_type(formation_type f_type);
		void update_position_offset();
		void get_coordinator_at();
		void position_offset_wrt_direction();
		
		
		
	};
	
}