#pragma once
#include <glm/ext/quaternion_float.hpp>
#include <string>
#include "iPhysicsComponent.h"
#include <glm/vec3.hpp>

namespace nPhysics
{
	struct sBumperDef
	{
		float Mass;
		float Radius;
		glm::vec3 Position;
		glm::vec3 half_length;
		glm::quat orientation;
		std::string AiType;
	};

	class iBumperComponent : public iPhysicsComponent
	{
	public:
		virtual ~iBumperComponent() {}

	protected:
		iBumperComponent(const sBumperDef& bumper_def) : iPhysicsComponent(eComponentType::BUMPER) {}

	};
}