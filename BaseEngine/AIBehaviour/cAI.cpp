#include "cAI.h"
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>


cAI::cAI()
{
	//coordinator = new cCoordinator();
	is_reached = false;
}

glm::vec3 cAI::seekR(glm::vec3 targetPos, cGameObject* aiObj,bool& is_ended)
{
	glm::vec3 desieredVelocity = targetPos - aiObj->m_physics_component->GetPosition();

	float dist = desieredVelocity.length();

	glm::vec3 direction = glm::normalize(desieredVelocity);

	if (dist < slowingRadius)
	{
		desieredVelocity = direction * maxVelocityBoid * (dist / slowingRadius);
	}
	else
	{
		desieredVelocity = direction * maxVelocityBoid;
	}
	glm::vec3 steer = desieredVelocity - aiObj->m_velocity;
	float stop_distance = glm::distance(targetPos, aiObj->m_physics_component->GetPosition());
	if (stop_distance < 1.5f)
		is_ended = true;

	return steer;
}
glm::vec3 cAI::fleeR(glm::vec3 targetPos, cGameObject* aiObj)
{
	glm::vec3 desieredVelocity = aiObj->m_physics_component->GetPosition() - targetPos;

	float dist = desieredVelocity.length();

	glm::vec3 direction = glm::normalize(desieredVelocity);

	if (dist < slowingRadius)
	{
		desieredVelocity = direction * maxVelocityBoid * (dist / slowingRadius);
	}
	else
	{
		desieredVelocity = direction * maxVelocityBoid;
	}

	

	glm::vec3 steer = desieredVelocity - aiObj->m_velocity;

	
	return steer;
}


