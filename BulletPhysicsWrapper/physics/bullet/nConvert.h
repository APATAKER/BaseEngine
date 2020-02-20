#pragma once
#include "btBulletDynamicsCommon.h"
#include "../../../Include/glm/vec3.hpp"
#include "../../../Include/glm/mat4x4.hpp"

namespace nConvert
{
	inline btVector3 ToBullet(const glm::vec3& vec)
	{
		return btVector3(vec.x, vec.y, vec.z);
	}

	inline glm::vec3 ToSimple(const btVector3& vec)
	{
		return glm::vec3(vec.x(), vec.y(), vec.z());
	}
	 inline  void ToSimple(const btTransform& transformIn,glm::mat4& transformOut)
	{
		 transformIn.getOpenGLMatrix(&transformOut[0][0]);
	}
	
}
