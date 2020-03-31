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
	virtual void SetPosition(const glm::vec3& pos) = 0;
	virtual glm::vec3 GetPosition() = 0;
	virtual void JumpForce(const glm::vec3& force) = 0;
	virtual void ApplyRotation(const glm::vec3& rotation) = 0;
	virtual int GetNumberOfPhysicalObject() = 0;
	virtual bool GetTransform(int index,glm::mat4& transformOut) = 0;
		
protected:
		iPhysicsComponent(eComponentType componentType)
			: mComponentType(componentType){}


		// common place for all the physics data
		
		eComponentType mComponentType;
	};
}
