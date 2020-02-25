#pragma once

#include <vector>

#include "cRigidBody.h"

namespace physLib
{
	
class cAI
{
	struct wanderDetails
	{
		wanderDetails(glm::vec3 wanderPt, float radius);
		glm::vec3 wanderPt;
		float radius;
	};
	float slowingRadius = 10.0f;
	float maxVelocity = 50.0f;
public:
	
	void seek(cRigidBody* target, cRigidBody* aiObj, double deltatime);
	void seek(glm::vec3 targetPos, cRigidBody* aiObj, double deltatime);
	void pursue(cRigidBody* target, cRigidBody* aiObj, double deltatime);
	void evade(cRigidBody* target, cRigidBody* aiObj, double deltatime);
	void wander(int& flag, std::vector<wanderDetails> wanderPts, cRigidBody* aiObj, double deltatime);
	void flee(cRigidBody* target, cRigidBody* aiObj, double deltatime);
};
}