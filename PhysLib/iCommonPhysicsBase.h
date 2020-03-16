#pragma once
#include <glm/vec3.hpp>


namespace physLib
{
	class iCommonPhysicsBase
	{
	public:
		virtual ~iCommonPhysicsBase() {};
		virtual void GetAabb(glm::vec3& minBoundsOut, glm::vec3& maxBoundsOut) = 0;
	};
}
