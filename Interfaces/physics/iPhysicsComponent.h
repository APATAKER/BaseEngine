#pragma once
#include "eComponentType.h"
#include <glm/mat4x4.hpp>

namespace nPhysics
{
	class iPhysicsComponent
	{
	public:
		virtual ~iPhysicsComponent() {}
		inline const eComponentType& GetComponentType() { return mComponentType; }

		
	virtual void GetTransform(glm::mat4& transformOut) = 0;
	virtual void ApplyForce(const glm::vec3& force) = 0;
	

protected:
		iPhysicsComponent(eComponentType componentType)
			: mComponentType(componentType){}


		// common place for all the physics data
		
		eComponentType mComponentType;
	};
}
