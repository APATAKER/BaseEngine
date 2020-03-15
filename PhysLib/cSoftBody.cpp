#include "cSoftBody.h"

#include "nCollide.h"
#include "shapes.h"

namespace physLib
{
	cSoftBody::cNode::cNode(const sSoftBodyNodeDef& node_def)
		:Position(node_def.Position),Mass(node_def.Mass),Velocity(glm::vec3(0,0,0)),Acceleration(glm::vec3(0,0,0)),Radius(5.f)
	{
		PreviousPosition = Position;
	}

	void cSoftBody::cNode::CalculateRadius()
	{
		float smallestDistance = FLT_MAX;
		size_t numNeighbors = this->Springs.size();
		for(size_t idx = 0;idx<numNeighbors;idx++)
		{
			
			// is this the smallest distance?
			float dist = glm::distance(Springs[idx]->GetOther(this)->Position, Position);
			if(dist < smallestDistance)
			{
				smallestDistance = dist;
			}
			
		}
		Radius = smallestDistance * 0.5f; // TODO: Tweak this value, maybe make it a constant somewhere?
	}

	bool cSoftBody::cNode::IsNeighbor(cNode* other)
	{
		size_t numSprings = Springs.size();
		for (size_t idx = 0; idx < numSprings; idx++)
		{
			if (Springs[idx]->GetOther(this) == other)
			{
				return true;
			}
		}
		return false;
	}
	
	cSoftBody::cSpring::cSpring(cNode* nodeA, cNode* nodeB, float springConstant)
		:NodeA(nodeA),NodeB(nodeB),RestingLength(glm::distance(nodeA->Position,nodeB->Position)),SpringConstant(springConstant),SpringForceAtoB(glm::vec3(0,0,0))
	{
		
	}

	void cSoftBody::cSpring::UpdateSpringForce()
	{
		// HOOKS LAW!!!!
		glm::vec3 sep = NodeB->Position - NodeA->Position;
		float dist = glm::length(sep);
		float x = dist - RestingLength;
		SpringForceAtoB = -glm::normalize(sep) * x * SpringConstant;
		
	}

	void cSoftBody::cSpring::ApplyForceToNodes()
	{
		if(!NodeB->isFixed())
		{
			NodeB->Acceleration += SpringForceAtoB / NodeB->Mass;
			
		}
		
		if(!NodeA->isFixed())
		{
			NodeA->Acceleration -= SpringForceAtoB / NodeA->Mass;
			
		}
	}

	cSoftBody::cNode* cSoftBody::cSpring::GetOther(cNode* node)
	{
		return node == NodeA ? NodeB : NodeA;
	}
	
	cSoftBody::cSoftBody(const sSoftBodyDef& def)
		:cCollisionBody(eBodyType::soft)
	{
		size_t numNodes = def.Nodes.size();
		mNodes.resize(numNodes);
		for (size_t idx = 0; idx < numNodes; idx++)
		{
			mNodes[idx] = new cNode(def.Nodes[idx]);
		}
		size_t numSprings = def.Springs.size();
		mSprings.resize(numSprings);
		for (size_t idx = 0; idx < numSprings; idx++)
		{
			mSprings[idx] = new cSpring(mNodes[def.Springs[idx].first],mNodes[def.Springs[idx].second],def.SpringConstant);
			mNodes[def.Springs[idx].first]->Springs.push_back(mSprings[idx]);
			mNodes[def.Springs[idx].second]->Springs.push_back(mSprings[idx]);
		}
		

	}
	cSoftBody::~cSoftBody()
	{
		size_t numNodes = mNodes.size();
		mNodes.resize(numNodes);
		for (size_t idx = 0; idx < numNodes; idx++)
		{
			delete mNodes[idx];
		}
		mNodes.clear();
		size_t numSprings = mSprings.size();
		mSprings.resize(numSprings);
		for (size_t idx = 0; idx < numSprings; idx++)
		{
			delete mSprings[idx];
		}
		mSprings.clear();
	}

	void cSoftBody::GetTransform(int index, glm::mat4& transformOut)
	{
		glm::mat4 translation_transform = glm::translate(glm::mat4(1.f), mNodes[index]->Position);
		transformOut *= translation_transform;
	}

	void cSoftBody::ClearAccelerations()
	{
		size_t numNodes = mNodes.size();
		for(size_t idx =0;idx<numNodes;idx++)
		{
			setAcceleration(glm::vec3(0, 0, 0));
		}
	}
	glm::vec3 cSoftBody::GetVelocity()
	{
		return glm::vec3();
	}
	glm::vec3 cSoftBody::GetAccelerations()
	{
		return glm::vec3();
	}
	std::string cSoftBody::GetAiType()
	{
		return std::string();
	}
	void cSoftBody::setVelocity(glm::vec3 velocity)
	{
	}
	void cSoftBody::setAcceleration(glm::vec3 accl)
	{
		
	}
	size_t cSoftBody::NumNodes()
	{
		return mNodes.size();
	}
	bool cSoftBody::GetNodeRadius(size_t index, float& radiusOut)
	{
		
		if (index >= mNodes.size())return  false;
		radiusOut = mNodes[index]->Radius;
		return true;
	}
	bool cSoftBody::GetNodePosition(size_t index, glm::vec3& positionOut)
	{
		if (index >= mNodes.size())return  false;
		positionOut = mNodes[index]->Position;
		return true;
	}
	bool cSoftBody::collideSpherecloth(cRigidBody* sphere)
	{
		std::vector<cNode*> closeNodes;
		glm::vec3 spherePosition = sphere->GetPosition();
		
		glm::vec3 spherePreviousPosition = sphere->GetPreviousPosition();
		const auto sphereShape = dynamic_cast<cSphere*>(sphere->GetShape());
		float sphereRadius = sphereShape->GetRadius();
		float sphereMass = 1.f;
		glm::vec3 sphereVelocity = sphere->GetVelocity();
		//sphere->GetVelocity(sphereVelocity);

		for (size_t idx = 0; idx < this->mNodes.size(); idx++)
		{
			float distanceSphereToNode = glm::distance(spherePosition, mNodes[idx]->Position);
			float collisionDistance = sphereRadius + mNodes[idx]->Radius + 2.0f;
			if (distanceSphereToNode < collisionDistance)
			{
				closeNodes.push_back(mNodes[idx]);
			}
		}

		for (size_t idx = 0; idx < closeNodes.size(); idx++)
		{
			glm::vec3 cA = spherePreviousPosition;
			glm::vec3 cB = closeNodes[idx]->PreviousPosition;
			glm::vec3 vA = spherePosition - spherePreviousPosition;
			glm::vec3 vB = closeNodes[idx]->Position - closeNodes[idx]->PreviousPosition;
			float rA = sphereRadius;
			//closeNodes[idx]->CalculateRadius();
			float rB = closeNodes[idx]->Radius;
			float t(0.0f);

			int result = nCollide::intersect_moving_sphere_sphere(cA, rA, vA, cB, rB, vB, t);
			if (result == 0)
			{
				// no collision
				continue;
			}

			// get the masses

			float ma = sphereMass;
			float mb = closeNodes[idx]->Mass;
			float mt = ma + mb;

			if (result == -1)
			{
				// already colliding

				float initialDistance = glm::distance(spherePreviousPosition, closeNodes[idx]->PreviousPosition);
				float targetDistance = rA + rB;

				glm::vec3 impulseToA = glm::normalize(spherePreviousPosition - closeNodes[idx]->PreviousPosition);
				impulseToA *= (targetDistance - initialDistance);

				// just push the cloth out of the way
				//bodyA->mPosition = bodyA->mPreviousPosition;
				closeNodes[idx]->Position = closeNodes[idx]->PreviousPosition;
				// apply the impulse
				//bodyA->mVelocity += impulseToA * (mb / mt);
				closeNodes[idx]->Velocity -= impulseToA * (ma / mt);

				//integrate
				//IntegrateRigidBody(bodyA, mDeltaTime);
				//IntegrateRigidBody(bodyB, mDeltaTime);

				return true;
			}

			// collided

			// everybody to ones
			// just push the cloth
			//bodyA->mPosition = bodyA->mPreviousPosition + vA * t;
			closeNodes[idx]->Position = closeNodes[idx]->PreviousPosition + vB * t;

			vA = sphereVelocity;
			vB = closeNodes[idx]->Velocity;

			float c = 0.2f;
			//bodyA->mVelocity = (c * mb * (vB - vA) + ma * vA + mb * vB) / mt;
			closeNodes[idx]->Velocity = ((c * ma * (vA - vB) + ma * vA + mb * vB)) / mt;

			// integrate
			//IntegrateRigidBody(bodyA, mDeltaTime * (1.0f - t));
			//IntegrateRigidBody(bodyB, mDeltaTime * (1.0f - t));
		}
		return true;
	}
	bool cSoftBody::collideNodeNode(cNode* bodyA, cNode* bodyB)
	{
		glm::vec3 cA = bodyA->PreviousPosition;
		glm::vec3 cB = bodyB->PreviousPosition;
		glm::vec3 vA = bodyA->Position - bodyA->PreviousPosition;
		glm::vec3 vB = bodyB->Position - bodyB->PreviousPosition;
		float rA = bodyA->Radius;
		float rB = bodyB->Radius;
		float t(0.0f);

		int result = nCollide::intersect_moving_sphere_sphere(cA, rA, vA, cB, rB, vB, t);
		if (result == 0)
		{
			// no collision
			return false;
		}

		// get the masses
		float ma = bodyA->Mass;
		float mb = bodyB->Mass;
		float mt = ma + mb;

		if (result == -1)
		{
			// already colliding

			float initialDistance = glm::distance(bodyA->PreviousPosition, bodyB->PreviousPosition);
			float targetDistance = rA + rB;

			glm::vec3 impulseToA = glm::normalize(bodyA->PreviousPosition - bodyB->PreviousPosition);
			impulseToA *= (targetDistance - initialDistance);

			// back to ones
			bodyA->Position = bodyA->PreviousPosition;
			bodyB->Position = bodyB->PreviousPosition;
			// apply the impulse
			bodyA->Velocity += impulseToA * (mb / mt);
			bodyB->Velocity -= impulseToA * (ma / mt);

			//integrate

			//IntegrateRigidBody(bodyA, mDeltaTime);
			//IntegrateRigidBody(bodyB, mDeltaTime);

			return true;
		}

		// collided

		// everybody to ones
		// rewind to point of collision
		bodyA->Position = bodyA->PreviousPosition + vA * t;
		bodyB->Position = bodyB->PreviousPosition + vB * t;

		vA = bodyA->Velocity;
		vB = bodyB->Velocity;

		float c = 0.2f;
		bodyA->Velocity = (c * mb * (vB - vA) + ma * vA + mb * vB) / mt;
		bodyB->Velocity = (c * ma * (vA - vB) + ma * vA + mb * vB) / mt;

		// integrate
		//IntegrateRigidBody(bodyA, mDeltaTime * (1.0f - t));
		//IntegrateRigidBody(bodyB, mDeltaTime * (1.0f - t));

		return true;
	}
	void cSoftBody::Intergrate(float dt, const glm::vec3& gravity)
	{
		size_t numNodes = mNodes.size();
		// STEP 1 : start with gravity
		for(size_t idx =0;idx<numNodes;idx++)
		{
			mNodes[idx]->Acceleration = gravity;
		}
		// STEP 2 : Accimulate spring forces based on current positions
		size_t numSprings = mSprings.size();
		for(size_t idx =0;idx<numSprings;idx++)
		{
			mSprings[idx]->UpdateSpringForce();
			mSprings[idx]->ApplyForceToNodes();
		}
		// STEP 3 : Intergrate the nodes!!
		for(size_t idx=0;idx<numNodes;idx++)
		{
			cNode* node = mNodes[idx];
			if (node->isFixed()) continue;
			node->Velocity += node->Acceleration * dt;
			node->Position += node->Velocity * dt;
			// Dampen
			node->Velocity *= glm::pow(0.6f, dt);
		}
		// STEP 4 : DO Internal collision
		for (size_t idxA = 0; idxA < numNodes - 1; idxA++)
		{
			for (size_t idxB = idxA + 1; idxB < numNodes; idxB++)
			{
				if (mNodes[idxA]->IsNeighbor(mNodes[idxB]))
				{
					continue;
				}
				if (collideNodeNode(mNodes[idxA], mNodes[idxB]))
				{
				}
			}
		}
		
	}
	void cSoftBody::IntegrateNode(cNode* node)
	{
		if(node->isFixed()) return ;
		//TODO: for me
	}
}
