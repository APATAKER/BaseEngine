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
		glm::quat orientation;
		std::string AiType;
	};

	class iFlipperComponent : public iPhysicsComponent
	{
	public:
		virtual ~iFlipperComponent() {}

	protected:
		iFlipperComponent(const sFlipperDef& flipper_def) : iPhysicsComponent(eComponentType::FLIPPER) {}

	};
}
