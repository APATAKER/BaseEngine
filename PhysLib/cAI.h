#pragma once

#include <vector>

#include "cRigidBody.h"
#include "cCoordinator.h"

namespace physLib
{
	
class cAI
{
	friend class cWorld;
	friend class cCoordinator;
	cAI();
	
	struct wanderDetails
	{
		wanderDetails(glm::vec3 wanderPt, float radius);
		glm::vec3 wanderPt;
		float radius;
	};
	float slowingRadius = 10.0f;
	float maxVelocityCoord = 50.0f;
	float maxVelocityBoid = 100.0f;
	bool is_reached;
	bool on_path;
	bool is_reverse;
	bool is_flock;
	//int current_path = 0;
	int neighbourCount = 0;
	glm::vec3 totalFlee = glm::vec3(0, 0, 0);
	float separationRadius = 10.0f;
	glm::vec3 totalVelocity = glm::vec3(0, 0, 0);
	float alignmentRadius = 15.0f;
	glm::vec3 totalPosition = glm::vec3(0, 0, 0);
	float cohesionRadius = 15.0f;

	glm::vec3 direction_coord_to_path_destination;
	
	cCoordinator* coordinator;

	
public:

	void formation(cCoordinator* coordinator, std::vector<cRigidBody*> boids);
	void pathfollow(cCoordinator* coordinator, std::vector<cRigidBody*> boids);
	glm::vec3 separation(cRigidBody* currentBoid, std::vector<cRigidBody*> boids);
	glm::vec3 alignment(cRigidBody* currentBoid, std::vector<cRigidBody*> boids);
	glm::vec3 cohesion(cRigidBody* currentBoid, std::vector<cRigidBody*> boids);
	void flock(std::vector<cRigidBody*> boids);
	glm::vec3 seekR(glm::vec3 targetPos, cRigidBody* aiObj);
	glm::vec3 fleeR(glm::vec3 targetPos, cRigidBody* aiObj);

	void rotateSlerp(double deltatime);

	

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