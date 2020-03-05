#pragma once
#include <glm/vec3.hpp>

#include "eBodyType.h"
#include "string"

namespace physLib
{
	class cCollisionBody
	{
	public:
		virtual ~cCollisionBody(){}

		inline  const eBodyType& GetBodyType() { return mBodyType; }

		inline void setUserPointer(void* userpointer) { mUserPointer = userpointer; }

		inline void* GetUserPointer() { return mUserPointer; }

		virtual void ClearAccelerations() = 0;
		virtual glm::vec3 GetVelocity() = 0;
		virtual glm::vec3 GetAccelerations() = 0;
		virtual std::string GetAiType() = 0;
		virtual void setVelocity(glm::vec3 velocity) = 0;
		virtual void setAcceleration(glm::vec3 accl) = 0;

	protected:
		cCollisionBody(eBodyType bodyType)
			:mBodyType(bodyType),mUserPointer(0)
		{
			
		}

	private:
		eBodyType mBodyType;
		void* mUserPointer;

		cCollisionBody() = delete;
		cCollisionBody(const cCollisionBody& other) = delete;
	};
}
