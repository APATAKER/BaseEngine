#pragma once

#include <vector>

#include "cRigidBody.h"
#include "cCoordinator.h"

namespace physLib
{
	
class cAI
{
	friend class cWorld;
	cAI();
	
	struct wanderDetails
	{
		wanderDetails(glm::vec3 wanderPt, float radius);
		glm::vec3 wanderPt;
		float radius;
	};
	float slowingRadius = 10.0f;
	float maxVelocityCoord = 25.0f;
	float maxVelocityBoid = 100.0f;
	bool is_reached;
	int current_path = 1;
	cCoordinator* coordinator;

	
public:

	void formation(cCoordinator* coordinator, std::vector<cRigidBody*> boids);
	void pathfollow(cCoordinator* coordinator, std::vector<cRigidBody*> boids);
	glm::vec3 seekR(glm::vec3 targetPos, cRigidBody* aiObj);

	void aiupdate(std::vector<cRigidBody*> boids,double deltatime,const float maxVelocity);
	void aiupdate(cRigidBody* boid, double deltatime, const float maxVelocity);
	// OLD AI functions
	void seek(cRigidBody* target, cRigidBody* aiObj, double deltatime);
	void seek(glm::vec3 targetPos, cRigidBody* aiObj, double deltatime);
	void pursue(cRigidBody* target, cRigidBody* aiObj, double deltatime);
	void evade(cRigidBody* target, cRigidBody* aiObj, double deltatime);
	void wander(int& flag, std::vector<wanderDetails> wanderPts, cRigidBody* aiObj, double deltatime);
	void flee(cRigidBody* target, cRigidBody* aiObj, double deltatime);

};
}