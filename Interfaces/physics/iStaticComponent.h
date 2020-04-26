#pragma once
#include <glm/ext/quaternion_float.hpp>
#include <glm/vec3.hpp>
#include <string>


#include "iPhysicsComponent.h"


namespace nPhysics
{
	struct sStaticDef
	{
		float Mass;
		float Radius;
		glm::vec3 Position;
		glm::vec3 half_length;
		glm::vec3 pivot;
		glm::quat orientation;
		std::string AiType;
	};

	class iStaticComponent : public iPhysicsComponent
	{
	public:
		virtual ~iStaticComponent() {}

	protected:
		iStaticComponent(const sStaticDef& static_def) : iPhysicsComponent(eComponentType::STATIC) {}

	};
}
