#include "cAI.h"
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>


physLib::cAI::cAI()
{
	coordinator = new physLib::cCoordinator();
	is_reached = false;
}

void physLib::cAI::formation(cCoordinator* coordinator, std::vector<cRigidBody*> boids)
{
	if (coordinator->formation_type_ == formation_type::square)
	{
		for (int i = 0; i < coordinator->position_offset_.size(); i++)
		{
			boids[i]->mSteerForce = seekR(coordinator->position_offset_[i], boids[i]);
			//seek(coordinator->position_offset_[i], boids[i], deltatime);
		}
	}
	else
	if (coordinator->formation_type_ == formation_type::line)
	{
		for (int i = 0; i < coordinator->position_offset_.size(); i++)
		{

			//seek(coordinator->position_offset_[i], boids[i], deltatime);
			boids[i]->mSteerForce = seekR(coordinator->position_offset_[i], boids[i]);
		}
		
	}
	else
	if (coordinator->formation_type_ == formation_type::v)
	{
		for (int i = 0; i < coordinator->position_offset_.size(); i++)
		{

			//seek(coordinator->position_offset_[i], boids[i], deltatime);
			boids[i]->mSteerForce = seekR(coordinator->position_offset_[i], boids[i]);
		}
	}else
	if (coordinator->formation_type_ == formation_type::circle)
	{
		for (int i = 0; i < coordinator->position_offset_.size(); i++)
		{

			//seek(coordinator->position_offset_[i], boids[i], deltatime);
			boids[i]->mSteerForce = seekR(coordinator->position_offset_[i], boids[i]);
		}
	}else
	if (coordinator->formation_type_ == formation_type::rows)
	{
		for (int i = 0; i < coordinator->position_offset_.size(); i++)
		{

			//seek(coordinator->position_offset_[i], boids[i], deltatime);
			boids[i]->mSteerForce = seekR(coordinator->position_offset_[i], boids[i]);
		}
	}
}

void physLib::cAI::pathfollow(cCoordinator* coordinator, std::vector<cRigidBody*> boids)
{
	if(coordinator->current_path<0)
	{
		coordinator->current_path = 4;
		//coordinator->coordinator->mOrientation = coordinator->coordinator->safeQuatLookAt(coordinator->path_[current_path], coordinator->coordinator->mPosition, glm::vec3(0, 1, 0));
	}
	if(coordinator->current_path > 4)
	{
		coordinator->current_path = 0;
		//coordinator->coordinator->mOrientation = coordinator->coordinator->safeQuatLookAt(coordinator->path_[current_path], coordinator->coordinator->mPosition, glm::vec3(0, 1, 0));
	}
	
	float distance_between_checkpoint_coord_path = glm::distance(coordinator->path_[coordinator->current_path],coordinator->coordinator->mPosition);
	//float distance_between_checkpoint_coord_revpath = glm::distance(coordinator->rev_path_[current_path],coordinator->coordinator->mPosition);

	//coordinator->get_coordinator_at();
	
	/*glm::quat rotationadjust = coordinator->coordinator->safeQuatLookAt(coordinator->path_[coordinator->current_path], coordinator->coordinator->mPosition, glm::vec3(0, 1, 0));
	coordinator->coordinator->mOrientation = rotationadjust;*/
	
	if (!is_reached)
	{
		if (!is_reverse)
		{
			
			//coordinator->coordinator->mOrientation = coordinator->coordinator->safeQuatLookAt(coordinator->path_[current_path],coordinator->coordinator->mPosition, glm::vec3(0, 1, 0));
			coordinator->coordinator->mSteerForce = seekR(coordinator->path_[coordinator->current_path], coordinator->coordinator);
			
			
			
			if (distance_between_checkpoint_coord_path < 5)
			{
				is_reached = true;
				coordinator->current_path++;
				if (coordinator->current_path < coordinator->path_.size())
				{
					is_reached = false;
				}
				else
				{
					coordinator->current_path = 0;
					is_reached = false;
				}
			}
		}
		else
		{
			
			if(coordinator->current_path<0)
			{
				coordinator->current_path = 4;
			}
			coordinator->coordinator->mSteerForce = seekR(coordinator->path_[coordinator->current_path], coordinator->coordinator);
			if (distance_between_checkpoint_coord_path < 5)
			{
				is_reached = true;
				coordinator->current_path--;
				if (coordinator->current_path < coordinator->path_.size())
				{
					is_reached = false;
				}
				else
				{
					coordinator->current_path = 4;
					is_reached = false;
				}
			}
		}
	}
	
	
	coordinator->update_position_offset();
}

glm::vec3 physLib::cAI::separation(cRigidBody* currentBoid, std::vector<cRigidBody*> boids)
{

	for (int i = 0; i < boids.size(); i++)
	{
		//float dist = Distance(currentPosition, boids[i].position);
		float dist = glm::distance(currentBoid->mPosition, boids[i]->mPosition);
		if ((dist > 0) && (dist < separationRadius))
		{
			glm::vec3 fleeVector = currentBoid->mPosition - boids[i]->mPosition;
			fleeVector = normalize(fleeVector);
			fleeVector /= dist;
			totalFlee += fleeVector;
			neighbourCount++;
		}
	}
	glm::vec3 steer = glm::vec3(0, 0, 0);
	if (neighbourCount > 0)
	{
		glm::vec3 desired_velocity = totalFlee / (float)neighbourCount;
		desired_velocity = normalize(desired_velocity);
		desired_velocity *= maxVelocityBoid;
		steer = desired_velocity - currentBoid->mVelocity;
	}
	return  steer;
	
}
glm::vec3 physLib::cAI::alignment(cRigidBody* currentBoid, std::vector<cRigidBody*> boids)
{
	
	for (int i = 0; i < boids.size(); i++)
	{
		float dist = glm::distance(currentBoid->mPosition, boids[i]->mPosition);
		if ((dist > 0) && (dist < alignmentRadius)) 
		{
			totalVelocity += boids[i]->mVelocity;
			neighbourCount++;
		}
	}
	glm::vec3 steerForce = glm::vec3(0, 0, 0);
	if (neighbourCount > 0) 
	{
		glm::vec3 desiredVelocity = totalVelocity / (float)neighbourCount;
		desiredVelocity = normalize(desiredVelocity);
		desiredVelocity *= maxVelocityBoid;
		steerForce = desiredVelocity - currentBoid->mVelocity;
		//steerForce *= maxForce;
	}
	return steerForce;
}
glm::vec3 physLib::cAI::cohesion(cRigidBody* currentBoid, std::vector<cRigidBody*> boids)
{

	for (int i = 0; i < boids.size(); i++) {
		float dist = glm::distance(currentBoid->mPosition, boids[i]->mPosition);
		if ((dist > 0) && (dist < cohesionRadius)) {
			totalPosition += boids[i]->mPosition;
			neighbourCount++;
		}

	}
	glm::vec3 steerForce = glm::vec3(0, 0, 0);

	if (neighbourCount > 0) {
		glm::vec3 target = totalPosition / (float)neighbourCount;
		steerForce = seekR(target,currentBoid);
	}
	return steerForce;

}
void physLib::cAI::flock(std::vector<cRigidBody*> boids)
{
	for(int i=0;i<boids.size();i++)
	{
		glm::vec3 sep = separation(boids[i], boids);
		glm::vec3 align = alignment(boids[i], boids);
		glm::vec3 coh = cohesion(boids[i], boids);

		sep *= 0.25f;
		align *= 0.25;
		coh *= 0.5;
		
		boids[i]->mSteerForce = sep + align + coh;
	}
}
void physLib::cAI::seek(cRigidBody* target, cRigidBody* aiObj, double deltatime)
{
	glm::vec3 desieredVelocity = target->mPosition - aiObj->mPosition;

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

	glm::vec3 steer = desieredVelocity - aiObj->mVelocity;

	//aiObj->m_physics_component->setVelocity(aiObj->m_physics_component->getVelocity() + (steer * (float)deltatime));
	aiObj->mVelocity += steer * (float)deltatime;

	if (aiObj->mVelocity.length() > maxVelocityBoid)
	{
		//aiObj->m_physics_component->setVelocity(normalize(aiObj->m_physics_component->getVelocity()) * maxVelocity);
		aiObj->mVelocity = glm::normalize(aiObj->mVelocity * maxVelocityBoid);
	}

}
glm::vec3 physLib::cAI::seekR(glm::vec3 targetPos, cRigidBody* aiObj)
{
	glm::vec3 desieredVelocity = targetPos - aiObj->mPosition;

	float dist = desieredVelocity.length();

	glm::vec3 direction = glm::normalize(desieredVelocity);

	if (dist < slowingRadius)
	{
		if(aiObj->mAiType == "coordinator")
		{
		desieredVelocity = direction * maxVelocityCoord * (dist / slowingRadius);
			
		}
		else
		{
		desieredVelocity = direction * maxVelocityBoid * (dist / slowingRadius);
			
		}
	}
	else
	{
		if (aiObj->mAiType == "coordinator")
		{
		desieredVelocity = direction * maxVelocityCoord;
			
		}
		else
		{
			
		desieredVelocity = direction * maxVelocityBoid;
		}
	}
	return desieredVelocity - aiObj->mVelocity;
}
glm::vec3 physLib::cAI::fleeR(glm::vec3 targetPos, cRigidBody* aiObj)
{
	glm::vec3 desieredVelocity = aiObj->mPosition - targetPos;

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

	glm::vec3 steer = desieredVelocity - aiObj->mVelocity;
	
	return steer;
}

void physLib::cAI::rotateSlerp(double deltatime)
{
	glm::quat rotationadjust = coordinator->coordinator->safeQuatLookAt(coordinator->path_[coordinator->current_path], coordinator->coordinator->mPosition, glm::vec3(0, 1, 0));
	glm::quat sample = glm::quat(glm::vec3(90.0f, 0.0f, 90.0f));
	glm::quat rotateThisFrame = glm::slerp(rotationadjust, sample,(float)deltatime);
	coordinator->coordinator->mOrientation = rotateThisFrame;

	for (int i = 0; i < coordinator->vehicles_.size(); i++)
	{
		
		coordinator->vehicles_[i]->mOrientation = coordinator->coordinator->mOrientation;
	}
}

void physLib::cAI::seek(glm::vec3 targetPos, cRigidBody* aiObj, double deltatime)
{
	glm::vec3 desieredVelocity = targetPos - aiObj->mPosition;

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

	glm::vec3 steer = desieredVelocity - aiObj->mVelocity;

	//aiObj->m_physics_component->setVelocity(aiObj->m_physics_component->getVelocity() + (steer * (float)deltatime));
	aiObj->mVelocity += steer * (float)deltatime;

	if (aiObj->mVelocity.length() > maxVelocityBoid)
	{
		//aiObj->m_physics_component->setVelocity(normalize(aiObj->m_physics_component->getVelocity()) * maxVelocity);
		aiObj->mVelocity = normalize(aiObj->mVelocity * maxVelocityBoid);
	}
}
void physLib::cAI::pursue(cRigidBody* target, cRigidBody* aiObj, double deltatime)
{
	//calculate the number of frames we are looking ahead
	glm::vec3 distance = target->mPosition - aiObj->mPosition;
	int T = (int)glm::length(distance) / (int)maxVelocityBoid;

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
		desiredVelocity = direction * maxVelocityBoid * (dist / slowingRadius);
	}
	else
	{
		desiredVelocity = direction * maxVelocityBoid;
	}

	/*calculate the steering force */
	glm::vec3 steer = desiredVelocity - aiObj->mVelocity;

	/* add steering force to current velocity*/
	//aiObj->m_physics_component->setVelocity(aiObj->m_physics_component->getVelocity() + (steer * (float)deltatime));
	aiObj->mVelocity += steer * (float)deltatime;
	
	if (aiObj->mVelocity.length() > maxVelocityBoid)
	{

		//aiObj->m_physics_component->setVelocity(glm::normalize(aiObj->m_physics_component->getVelocity()) * maxVelocity);
		aiObj->mVelocity = normalize(aiObj->mVelocity * maxVelocityBoid);
	}
}
void physLib::cAI::evade(cRigidBody* target, cRigidBody* aiObj, double deltatime)
{
	//calculate the number of frames we are looking ahead
	glm::vec3 distance = target->mPosition - aiObj->mPosition;
	int T = (int)glm::length(distance) / (int)maxVelocityBoid;

	glm::vec3 frame = glm::vec3(T);
	//the future target point the vehicle will pursue towards
	glm::vec3 futurePosition = target->mPosition + target->mVelocity * frame;

	/*calculates the desired velocity */
	glm::vec3 desiredVelocity = aiObj->mPosition - futurePosition;

	float dist = desiredVelocity.length();

	//desiredVelocity = glm::normalize(desiredVelocity);
	//desiredVelocity.Normalize();
	glm::vec3 direction = glm::normalize(desiredVelocity);

	desiredVelocity = direction * maxVelocityBoid;


	/*calculate the steering force */
	glm::vec3 steer = desiredVelocity - aiObj->mVelocity;

	/* add steering force to current velocity*/
	//aiObj->m_physics_component->setVelocity(aiObj->m_physics_component->getVelocity() + (steer * (float)deltatime));
	aiObj->mVelocity += steer * (float)deltatime;
	
	if (aiObj->mVelocity.length() > maxVelocityBoid)
	{

		//aiObj->m_physics_component->setVelocity(glm::normalize(aiObj->m_physics_component->getVelocity()) * maxVelocity);
		aiObj->mVelocity = normalize(aiObj->mVelocity * maxVelocityBoid);
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
		desieredVelocity = direction * maxVelocityBoid * (dist / slowingRadius);
	}
	else
	{
		desieredVelocity = direction * maxVelocityBoid;
	}

	glm::vec3 steer = desieredVelocity - aiObj->mVelocity;

	//aiObj->m_physics_component->setVelocity(aiObj->m_physics_component->getVelocity() + (steer * (float)deltatime));
	aiObj->mVelocity += steer * (float)deltatime;
	
	if (aiObj->mVelocity.length() > maxVelocityBoid)
	{
		//aiObj->m_physics_component->setVelocity(normalize(aiObj->m_physics_component->getVelocity()) * maxVelocity);
		aiObj->mVelocity = normalize(aiObj->mVelocity * maxVelocityBoid);
	}

}

void physLib::cAI::aiupdate(std::vector<cRigidBody*> boids, double deltatime,const float maxVelocity)
{
	for (int i = 0; i < boids.size(); i++)
	{
		boids[i]->mVelocity += boids[i]->mSteerForce * (float)deltatime;
		//boids[i]->mVelocity *= 2;
		if (boids[i]->mVelocity.length() > maxVelocity)
		{
			//aiObj->m_physics_component->setVelocity(normalize(aiObj->m_physics_component->getVelocity()) * maxVelocity);
			boids[i]->mVelocity = normalize(boids[i]->mVelocity * maxVelocity);
		}


		/*if(on_path)
		for (int in = 0; in < coordinator->position_offset_.size(); in++)
		{

			coordinator->position_offset_[in] += coordinator->coordinator->mPosition;
			coordinator->vehicles_[in]->mOrientation = coordinator->coordinator->mOrientation;
		}*/
		
	}
}

void physLib::cAI::aiupdate(cRigidBody* boid, double deltatime, const float maxVelocity)
{
	boid->mVelocity += boid->mSteerForce * (float)deltatime;
	if (boid->mVelocity.length() > maxVelocity)
	{
		//aiObj->m_physics_component->setVelocity(normalize(aiObj->m_physics_component->getVelocity()) * maxVelocity);
		boid->mVelocity = normalize(boid->mVelocity * maxVelocity);
	}

	//if (on_path)
	//{

	//	glm::quat rotationadjust = coordinator->coordinator->safeQuatLookAt(coordinator->path_[coordinator->current_path], coordinator->coordinator->mPosition, glm::vec3(0, 1, 0));
	//	//glm::quat rotateThisFrame = glm::slerp(rotationadjust, dt);
	//	coordinator->coordinator->mOrientation = rotationadjust;
	//}
}

