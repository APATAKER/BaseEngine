#include "cAI.h"
#include <glm/vec3.hpp>


void physLib::cAI::seek(cRigidBody* target, cRigidBody* aiObj, double deltatime)
{
	glm::vec3 desieredVelocity = target->mPosition - aiObj->mPosition;

	float dist = desieredVelocity.length();

	glm::vec3 direction = glm::normalize(desieredVelocity);

	if (dist < slowingRadius)
	{
		desieredVelocity = direction * maxVelocity * (dist / slowingRadius);
	}
	else
	{
		desieredVelocity = direction * maxVelocity;
	}

	glm::vec3 steer = desieredVelocity - aiObj->mVelocity;

	//aiObj->m_physics_component->setVelocity(aiObj->m_physics_component->getVelocity() + (steer * (float)deltatime));
	aiObj->mVelocity += steer * (float)deltatime;

	if (aiObj->mVelocity.length() > maxVelocity)
	{
		//aiObj->m_physics_component->setVelocity(normalize(aiObj->m_physics_component->getVelocity()) * maxVelocity);
		aiObj->mVelocity = glm::normalize(aiObj->mVelocity * maxVelocity);
	}

}
void physLib::cAI::seek(glm::vec3 targetPos, cRigidBody* aiObj, double deltatime)
{
	glm::vec3 desieredVelocity = targetPos - aiObj->mPosition;

	float dist = desieredVelocity.length();

	glm::vec3 direction = glm::normalize(desieredVelocity);

	if (dist < slowingRadius)
	{
		desieredVelocity = direction * maxVelocity * (dist / slowingRadius);
	}
	else
	{
		desieredVelocity = direction * maxVelocity;
	}

	glm::vec3 steer = desieredVelocity - aiObj->mVelocity;

	//aiObj->m_physics_component->setVelocity(aiObj->m_physics_component->getVelocity() + (steer * (float)deltatime));
	aiObj->mVelocity += steer * (float)deltatime;

	if (aiObj->mVelocity.length() > maxVelocity)
	{
		//aiObj->m_physics_component->setVelocity(normalize(aiObj->m_physics_component->getVelocity()) * maxVelocity);
		aiObj->mVelocity = normalize(aiObj->mVelocity * maxVelocity);
	}
}
void physLib::cAI::pursue(cRigidBody* target, cRigidBody* aiObj, double deltatime)
{
	//calculate the number of frames we are looking ahead
	glm::vec3 distance = target->mPosition - aiObj->mPosition;
	int T = (int)glm::length(distance) / (int)maxVelocity;

	glm::vec3 frame = glm::vec3(T);
	//the future target point the vehicle will pursue towards
	glm::vec3 futurePosition = target->mPosition + target->mVelocity * frame;

	/*calculates the desired velocity */
	glm::vec3 desiredVelocity = futurePosition - aiObj->mPosition;

	float dist = desiredVelocity.length();

	//desiredVelocity = glm::normalize(desiredVelocity);
	//desiredVelocity.Normalize();
	glm::vec3 direction = glm::normalize(desiredVelocity);

	if (dist < slowingRadius)
	{
		desiredVelocity = direction * maxVelocity * (dist / slowingRadius);
	}
	else
	{
		desiredVelocity = direction * maxVelocity;
	}

	/*calculate the steering force */
	glm::vec3 steer = desiredVelocity - aiObj->mVelocity;

	/* add steering force to current velocity*/
	//aiObj->m_physics_component->setVelocity(aiObj->m_physics_component->getVelocity() + (steer * (float)deltatime));
	aiObj->mVelocity += steer * (float)deltatime;
	
	if (aiObj->mVelocity.length() > maxVelocity)
	{

		//aiObj->m_physics_component->setVelocity(glm::normalize(aiObj->m_physics_component->getVelocity()) * maxVelocity);
		aiObj->mVelocity = normalize(aiObj->mVelocity * maxVelocity);
	}
}
void physLib::cAI::evade(cRigidBody* target, cRigidBody* aiObj, double deltatime)
{
	//calculate the number of frames we are looking ahead
	glm::vec3 distance = target->mPosition - aiObj->mPosition;
	int T = (int)glm::length(distance) / (int)maxVelocity;

	glm::vec3 frame = glm::vec3(T);
	//the future target point the vehicle will pursue towards
	glm::vec3 futurePosition = target->mPosition + target->mVelocity * frame;

	/*calculates the desired velocity */
	glm::vec3 desiredVelocity = aiObj->mPosition - futurePosition;

	float dist = desiredVelocity.length();

	//desiredVelocity = glm::normalize(desiredVelocity);
	//desiredVelocity.Normalize();
	glm::vec3 direction = glm::normalize(desiredVelocity);

	desiredVelocity = direction * maxVelocity;


	/*calculate the steering force */
	glm::vec3 steer = desiredVelocity - aiObj->mVelocity;

	/* add steering force to current velocity*/
	//aiObj->m_physics_component->setVelocity(aiObj->m_physics_component->getVelocity() + (steer * (float)deltatime));
	aiObj->mVelocity += steer * (float)deltatime;
	
	if (aiObj->mVelocity.length() > maxVelocity)
	{

		//aiObj->m_physics_component->setVelocity(glm::normalize(aiObj->m_physics_component->getVelocity()) * maxVelocity);
		aiObj->mVelocity = normalize(aiObj->mVelocity * maxVelocity);
	}
}
void physLib::cAI::wander(int& flag, std::vector<wanderDetails> wanderPts, cRigidBody* aiObj, double deltatime)
{
	/*cRigidBody* player = findGameObjectByFriendlyName(g_vec_pGameObjects, "player");
	glm::quat directionToFaceThePlayer = safeQuatLookAt(player->m_physics_component->getPosition(), aiObj->m_physics_component->getPosition(), glm::vec3(0, 1, 0));*/
	/*aiObj->setOrientation(directionToFaceThePlayer);
	aiObj->getCurrentDirection();*/
	//auto flag = 0;
	if (flag == 0)
	{
		seek(wanderPts[0].wanderPt, aiObj, deltatime);
		float dist = glm::length(aiObj->mPosition - wanderPts[0].wanderPt);
		if (dist < wanderPts[0].radius)
			flag = 1;
	}
	else if (flag == 1)
	{
		seek(wanderPts[1].wanderPt, aiObj, deltatime);
		float dist = glm::length(aiObj->mPosition - wanderPts[1].wanderPt);
		if (dist < wanderPts[1].radius)
			flag = 2;
	}
}
void physLib::cAI::flee(cRigidBody* target, cRigidBody* aiObj, double deltatime)
{
	glm::vec3 desieredVelocity = aiObj->mPosition - target->mPosition;

	float dist = desieredVelocity.length();

	glm::vec3 direction = glm::normalize(desieredVelocity);

	if (dist < slowingRadius)
	{
		desieredVelocity = direction * maxVelocity * (dist / slowingRadius);
	}
	else
	{
		desieredVelocity = direction * maxVelocity;
	}

	glm::vec3 steer = desieredVelocity - aiObj->mVelocity;

	//aiObj->m_physics_component->setVelocity(aiObj->m_physics_component->getVelocity() + (steer * (float)deltatime));
	aiObj->mVelocity += steer * (float)deltatime;
	
	if (aiObj->mVelocity.length() > maxVelocity)
	{
		//aiObj->m_physics_component->setVelocity(normalize(aiObj->m_physics_component->getVelocity()) * maxVelocity);
		aiObj->mVelocity = normalize(aiObj->mVelocity * maxVelocity);
	}

}

