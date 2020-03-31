#pragma once

#include <vector>

#include "../GameObject/cGameObject.h"

class cAI
{
public:
	cAI();

private:
	struct wanderDetails
	{
		wanderDetails(glm::vec3 wanderPt, float radius);
		glm::vec3 wanderPt;
		float radius;
	};
	float slowingRadius = 10.0f;
	float maxVelocityCoord = 50.0f;
	float maxVelocityBoid = 10.0f;
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


	
public:

	//void flock(std::vector<cRigidBody*> boids);
	glm::vec3 seekR(glm::vec3 targetPos, cGameObject* aiObj, bool& is_ended);
	glm::vec3 fleeR(glm::vec3 targetPos, cGameObject* aiObj);

};