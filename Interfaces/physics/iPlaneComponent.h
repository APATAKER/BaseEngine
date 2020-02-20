#pragma once

#include "iPhysicsComponent.h"

namespace nPhysics
{
	struct sPlaneDef
	{
		glm::vec3 position;
		float Constant;
		glm::vec3 Normal;

		sPlaneDef():Constant(0.0f),Normal(glm::vec3(0,0,0)),position(glm::vec3(0, 0, 0)){}
	};
	class iPlaneComponent : public iPhysicsComponent
	{
	public:
		virtual ~iPlaneComponent() = default;
		

	protected:
		iPlaneComponent(sPlaneDef plane_def) : iPhysicsComponent(eComponentType::PLANE) {}
	};
}