#pragma once

#include "iPhysicsComponent.h"
#include <glm/vec3.hpp>

namespace nPhysics
{
	struct sBallDef
	{
		float Mass;
		float Radius;
		glm::vec3 Position;
	};

	class iBallComponent : public iPhysicsComponent
	{
	public:
		virtual ~iBallComponent() {}
		
	protected:
		iBallComponent(const sBallDef& ball_def) : iPhysicsComponent(eComponentType::SPHERE) {}
	
	};
}
