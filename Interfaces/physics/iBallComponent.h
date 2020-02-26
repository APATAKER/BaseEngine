#pragma once

#include <glm/ext/quaternion_float.hpp>

#include "iPhysicsComponent.h"
#include <glm/vec3.hpp>

namespace nPhysics
{
	struct sBallDef
	{
		float Mass;
		float Radius;
		glm::vec3 Position;
		glm::quat orientation;
	};

	class iBallComponent : public iPhysicsComponent
	{
	public:
		virtual ~iBallComponent() {}
		
	protected:
		iBallComponent(const sBallDef& ball_def) : iPhysicsComponent(eComponentType::SPHERE) {}
	
	};
}
