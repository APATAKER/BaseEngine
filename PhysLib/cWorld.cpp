#include "cWorld.h"    // My header;
#include "nCollide.h"  // collision detection functions from
                       // REAL-TIME COLLISION DETECTION, ERICSON


namespace physLib
{
	cWorld::cWorld()
	{
		this->mDt = 0.f;
		this->mGravity = glm::vec3(0.f, -9.8f, 0.f);
	}

	cWorld::~cWorld()
	{	
	}
	
	void cWorld::GetGravity(glm::vec3& gravityOut)
	{
		gravityOut = this->mGravity;
	}

	void cWorld::SetGravity(const glm::vec3& gravityIn)
	{
		this->mGravity = gravityIn;
		
	}

	void cWorld::Update(float dt)
	{
	
		// 1) If we have no bodies, there's nothing to do... return.
		// 2) Integrate each body.
		// 3) Perform collision handling on each unique pair of bodies.
		// 4) Clear the acceleration of each rigid body.

		mDt = dt;
		auto numBodies = mBodies.size();
		
		if(numBodies == 0) //1)
		{
			return;
		}
		for(size_t idx = 0;idx <numBodies;idx++) //2)
		{
			mBodies[idx]->mAcceleration += (mGravity);
			IntegrateRigidBody(mBodies[idx], mDt);
		}
		
		for (size_t outerloopindex = 0; outerloopindex < numBodies-1; outerloopindex++) //3)
		{
			for (size_t innerloopindex = outerloopindex + 1; innerloopindex < numBodies; innerloopindex++)
			{
				Collide(mBodies[outerloopindex], mBodies[innerloopindex]);
			}
		}

		for (size_t idx = 0; idx < numBodies; idx++) //4)
		{
			mBodies[idx]->mAcceleration = glm::vec3(0.f, 0.f, 0.f);
		}
	}

	bool cWorld::AddRigidBody(cRigidBody* rigidBody)
	{
	
		// 1) Null check
		// 2) Check if we currently have this rigid body.
		//    If yes: Return false to indicate nothing was not added.
		//    If no:  Add it, then return true to indicate it was added.
		if(rigidBody == nullptr) //1)
		{
			return false;
		}
		for (int i = 0; i < mBodies.size(); i++)//2)
		{
			if (mBodies[i] == rigidBody)
				return false;
		}
		mBodies.push_back(rigidBody);
		
		return true; // rigidbody was added in vector mBodies
	}

	bool cWorld::RemoveRigidBody(cRigidBody* rigidBody)
	{
		
		// 1) Null check
		// 2) Check if we currently have this rigid body.
		//    If yes: Remove it, then return true to indicate it was removed.
		//    If no:  Return false to indicate nothing was removed.
		if (rigidBody == nullptr)  //1)
		{
			return false;
		}
		for (int i = 0; i < mBodies.size(); i++) //2)
		{
			if (mBodies[i] == rigidBody)
			{
				mBodies.erase(mBodies.begin() + (i - 1));
				return true;
			}
		}
		return false; // Nothing was removed
	}

	/*void cWorld::SetCollisionListener(nPhysics::iCollisionListener* collision_listener)
	{
		
	}*/

	void cWorld::IntegrateRigidBody(cRigidBody* body, float dt)
	{
	
		// 1) Static bodies are not to be integrated!
		// 2) Update the body's previous position.
		// 3) Do some integrating!
		// 4) Apply some linear damping 
		if(!body->mMass == 0) //1)
		{
			body->mPreviousPosition = body->mPosition;  //2)
			mIntegrator.RK4(body->mPosition, body->mVelocity, body->mAcceleration, dt); //3)
		}
		
	}
	bool cWorld::Collide(cRigidBody* bodyA, cRigidBody* bodyB)
	{
	
		// 1) Based on shape type, determine which specific collision handling
		//    method to use.
		// 2) Cast up the shapes, call the methods, return the result.

		eShapeType bodyAshape = bodyA->GetShapeType();  //1)
		eShapeType bodyBshape = bodyB->GetShapeType();

		if (bodyAshape == eShapeType::plane && bodyBshape == eShapeType::sphere)
		{
			const auto sphereShapeA = dynamic_cast<cPlane*>(bodyA->GetShape());
			const auto planeShapeB = dynamic_cast<cSphere*>(bodyB->GetShape());
			return CollideSpherePlane(bodyB, planeShapeB, bodyA, sphereShapeA);  //2)
		}
		 if (bodyAshape == eShapeType::sphere && bodyBshape == eShapeType::sphere)
		{
			 const auto sphereShapeA = dynamic_cast<cSphere*>(bodyA->GetShape());
			 const auto sphereShapeB = dynamic_cast<cSphere*>(bodyB->GetShape());
			return CollideSphereSphere(bodyA, sphereShapeA, bodyB, sphereShapeB);
		}
		
		return false;
		
		
	}

	bool cWorld::CollideSpherePlane(cRigidBody* sphereBody, cSphere* sphereShape,
		cRigidBody* planeBody, cPlane* planeShape)
	{

		// From our textbook, REAL-TIME COLLISION DETECTION, ERICSON
		// Use intersect_moving_sphere_plane(...inputs...outputs...)
		// to determine if:
		// case A: The sphere did not collide during the timestep.
		// case B: The sphere was already colliding at the beginning of the timestep.
		// case C: The sphere collided during the timestep.
		//
		// case A: Return false to indicate no collision happened.
		//
		// case B: Do the timestep again for this sphere after applying an
		//         impulse that should separate it from the plane.
		// 
		// 1) From our textbook, use closest_point_on_plane(..inputs..) to determine the 
		//    penetration-depth of the sphere at the beginning of the timestep.
		//    (This penetration-depth is the distance the sphere must travel during
		//    the timestep in order to escape the plane.)
		// 2) Use the sphere's center and the closest point on the plane to define
		//    the direction of our impulse vector.
		// 3) Use (penetration-depth / DT) to define the magnitude of our impulse vector.
		//    (The impulse vector is now distance/time ...a velocity!)
		// 4) Apply the impulse vector to sphere velocity.
		// 5) Reset the sphere position.
		// 6) Re-do the integration for the timestep.
		// 7) Return true to indicate a collision has happened.
		// 
		// 
		// case C: Rewind to the point of impact, reflect, then replay.
		//
		// 1) Use the outputs from the Ericson function to determine
		//    and set the sphere position to the point of impact.
		// 2) Reflect the sphere's velocity about the plane's normal vector.
		// 3) Apply some energy loss (to the velocity) in the direction
		//    of the plane's normal vector.
		// 4) Re-integrate the sphere with its new velocity over the remaining
		//    portion of the timestep.
		// 5) Return true to indicate a collision has happened.
		
		glm::vec3 c = sphereBody->mPreviousPosition;
		float r = sphereShape->GetRadius();
		glm::vec3 v = sphereBody->mPosition - sphereBody->mPreviousPosition;
		glm::vec3 n = planeShape->GetNormal();
		float d = planeShape->GetConstant();
		float t(0.f);
		glm::vec3 q;
		
		int result = nCollide::intersect_moving_sphere_plane(c, r, v, n, d, t, q);

		if(result == 0)    // Case A
		{
			//no collision
			return false;
		}
		if(result == -1)    // Case B
		{
			// Already collided at timestep
			glm::vec3 pointOnPlane = nCollide::closest_point_on_plane(sphereBody->mPreviousPosition, planeShape->GetNormal(), planeShape->GetConstant());
			float distance = glm::distance(sphereBody->mPreviousPosition, pointOnPlane);
			float targetDistance = r;
			glm::vec3 impluse = n * (targetDistance - distance) / mDt;
			sphereBody->mPosition = sphereBody->mPreviousPosition;
			sphereBody->mVelocity += impluse;
			float checkVelocity = glm::length(sphereBody->mVelocity);
			if(checkVelocity < 0.5f)
			{
				sphereBody->mVelocity = glm::vec3(0);
			}
			IntegrateRigidBody(sphereBody, mDt);
			return true;
		}
		// collision detected   Case C
		sphereBody->mVelocity = glm::reflect(sphereBody->mVelocity, planeShape->GetNormal());

		sphereBody->mVelocity *= .5f; // dampening effect
		glm::vec3 nComponent = (c+v*t);
		IntegrateRigidBody(sphereBody, mDt * (1.f - t));
		
		
		return true; 
	}

	bool cWorld::CollideSphereSphere(cRigidBody* bodyA, cSphere* shapeA,
		cRigidBody* bodyB, cSphere* shapeB)
	{
	
		// From our textbook, REAL-TIME COLLISION DETECTION, ERICSON
		// Use intersect_moving_sphere_sphere(...inputs...outputs...)
		// to determine if:
		// case A: The spheres don't collide during the timestep.
		// case B: The spheres were already colliding at the beginning of the timestep.
		// case C: The spheres collided during the timestep.
		//
		// case A: Return false to indicate no collision happened.
		//
		// case B: Do the timestep again for these spheres after
		//         applying an impulse that should separate them.
		// 
		// 1) Determine the penetration-depth of the spheres at the beginning of the timestep.
		//    (This penetration-depth is the distance the spheres must travel during
		//    the timestep in order to separate.)
		// 2) Use the sphere's centers to define the direction of our impulse vector.
		// 3) Use (penetration-depth / DT) to define the magnitude of our impulse vector.
		//    (The impulse vector is now distance/time ...a velocity!)
		// 4) Apply a portion of the impulse vector to sphereA's velocity.
		// 5) Apply a portion of the impulse vector to sphereB's velocity.
		//    (Be sure to apply the impulse in opposing directions.)
		// 6) Reset the spheres' positions.
		// 7) Re-do the integration for the timestep.
		// 8) Return true to indicate a collision has happened.
		// 
		// 
		// case C: 
		//
		// 1) Use the outputs from the Ericson function to determine
		//    and set the spheres positions to the point of impact.
		// 2) Use the inelastic collision response equations from
		//    Wikepedia to set they're velocities post-impact.
		// 3) Re-integrate the spheres with their new velocities
		//    over the remaining portion of the timestep.
		// 4) Return true to indicate a collision has happened.

		glm::vec3 cA = bodyA->mPreviousPosition;
		glm::vec3 cB = bodyB->mPreviousPosition;
		glm::vec3 vA = bodyA->mPosition - bodyA->mPreviousPosition;
		glm::vec3 vB = bodyB->mPosition - bodyB->mPreviousPosition;
		float rA = shapeA->GetRadius();
		float rB = shapeB->GetRadius();
		float t(0.f);
		int result = nCollide::intersect_moving_sphere_sphere(cA, rA, vA, cB, rB, vB, t);
		if(result ==0)  // Case A
		{
			//not colliding
			return false;
		}
		// getting mass
		float ma = bodyA->mMass;
		float mb = bodyB->mMass;
		float mt = ma + mb;
		
		if(result == -1)  //Case B
		{
			// already colliding
			float initialDistance = glm::distance(bodyA->mPreviousPosition, bodyB->mPreviousPosition);
			float targetDistance = rA + rB;

			glm::vec3 impluseToA = glm::normalize(bodyA->mPreviousPosition - bodyB->mPreviousPosition);
			impluseToA *= (targetDistance - initialDistance);

			bodyA->mPosition = bodyA->mPreviousPosition;
			bodyB->mPosition = bodyB->mPreviousPosition;

			bodyA->mVelocity += impluseToA * (mb / mt);
			bodyB->mVelocity -= impluseToA * (ma / mt);

			IntegrateRigidBody(bodyA, mDt);
			IntegrateRigidBody(bodyB, mDt);
			
			return true;
		}

		// Collision Detection  Case C
		bodyA->mPosition = bodyA->mPreviousPosition + vA * t;
		bodyB->mPosition = bodyB->mPreviousPosition + vB * t;

		vA = bodyA->mVelocity;
		vB = bodyB->mVelocity;
		
		float c = 0.2f;
		bodyA->mVelocity = (c * mb * (vB - vA) + ma * vA + mb * vB) / mt;
		bodyB->mVelocity = (c * mb * (vA - vB) + ma * vA + mb * vB) / mt;

		IntegrateRigidBody(bodyA, mDt * (1.f - t));
		IntegrateRigidBody(bodyB, mDt * (1.f - t));
		
		return true; 
	}
}