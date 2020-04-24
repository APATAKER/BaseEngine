#pragma once
#include <glm/ext/quaternion_float.hpp>
#include <glm/vec3.hpp>
#include <string>

#include "iPhysicsComponent.h"


namespace nPhysics
{
	struct sFlipperDef
	{
		float Mass;
		float Radius;
		glm::vec3 Position;
		glm::vec3 half_length;
		glm::vec3 pivot;
		glm::quat orientation;
		std::string AiType;
	};

	class iFlipperComponent : public iPhysicsComponent
	{
	public:
		virtual ~iFlipperComponent() {}
		virtual void GoToAngle(float angle, float dt, float target_velocity) = 0;
		virtual void disableMotor() = 0;
		
	protected:
		iFlipperComponent(const sFlipperDef& flipper_def) : iPhysicsComponent(eComponentType::FLIPPER) {}

	};
}
