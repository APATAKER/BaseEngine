#pragma once
#include "cRigidBody.h"   // cRigidBody class
#include <vector>         // std::vector to contain cRigidBody pointers
#include "shapes.h"       // all specific shape classes
#include "cIntegrator.h"  // cIntegrator class for timesteps
#include "cAI.h"


#define DLL_EXPORT __declspec(dllexport)

namespace physLib
{
	// cWorld
	// 
	// A rigid body simulator.
	// 
	// Pointers to cRigidBody instances can be added and removed.
	// 
	// cWorld will operate on the contained cRigidBody's during timesteps
	// triggered by calls to cWorld::Update(dt)
	// 
	// cWorld does not own cRigidBody pointers, and will not
	// delete them when it is deleted.
	class DLL_EXPORT cWorld
	{
	public:
		// Constructor
		// Create a cWorld
		// The world is initially empty, containing no rigid bodies.
		cWorld();

		// Destructor
		// Does not delete any cRigidBody pointers.
		~cWorld();

		// GetGravity
		// Get the gravity acceleration vector.
		void GetGravity(glm::vec3& gravityOut);

		// SetGravity
		// Set the gravity acceleration vector.
		// This will be applied to all rigid bodies during timesteps.
		void SetGravity(const glm::vec3& gravityIn);

		// Update
		// A single trigger to perform a single timestep.
		// All rigid bodies will go through integration, collision
		// detection and reaction, and have their internal values updated.
		void Update(float dt);

		// AddRigidBody
		// Add a rigid body to the world. 
		// Returns true if:
		//    There was an addition to the world.
		//    This particular rigid body was added to the world.
		//    (It must not have already been contained in the world)
		// Returns false if:
		//    There was no addition to the world.
		//    A null pointer is passed in.
		//    This particular rigid body is already in the world, hense not added again.
		bool AddRigidBody(cRigidBody* rigidBody);

		// RemoveRigidBody
		// Remove a rigid body from the world.
		// Returns true if:
		//    There was a removal from the world.
		//    This particular rigid body existed in the world and was removed.
		// Returns false if:
		//    There was no removal from the world.
		//    A null pointer was passed in.
		//    This particular rigid body was not in the world, hense not removed.
		bool RemoveRigidBody(cRigidBody* rigidBody);

		void AiSetup();


		void GetAiFormationType(int type);

		void SetCoordinator();

		//void SetCollisionListener(nPhysics::iCollisionListener* collision_listener);

	protected:

		// IntegrateRigidBody
		// Performs a single numerical integration step.
		// Safe for null pointers.
		// Safe for static rigid bodies.
		void IntegrateRigidBody(cRigidBody* body, float dt);

		// Collide
		// Entry point for collision detection.
		// Returns the result of specific Collide[shapeA][ShapeB] methods.
		bool Collide(cRigidBody* bodyA, cRigidBody* bodyB);

		// CollideSpherePlane
		// Handles collision detection and reaction between a sphere and a plane.
		// Returns true if a collision occured.
		// Returns false if no collision occured.
		bool CollideSpherePlane(cRigidBody* sphereBody, cSphere* sphereShape, 
			cRigidBody* planeBody, cPlane* planeShape);

		// CollideSphereSphere
		// Handles collision detection and reaction between two spheres.
		// Returns true if a collision occured.
		// Returns false if no collision occured.
		bool CollideSphereSphere(cRigidBody* bodyA, cSphere* shapeA,
			cRigidBody* bodyB, cSphere* shapeB);

		void AddBoids();


	private:

		// The delta time input cached during each Update(dt) call.
		float m_dt_;
		// Our handy dandy integrator.
		cIntegrator m_integrator_;
		// Acceleration due to gravity applied to each rigid body
		// during each timestep.
		glm::vec3 m_gravity_;
		// All the rigid bodies currently in the world.
		// Not owned by cWorld, will not be deleted in the destructor.
		std::vector<cRigidBody*> m_bodies_;

		cAI* ai;

		// Constructors not to be used.
		cWorld(const cWorld& other) = delete;
		cWorld& operator=(const cWorld& other) = delete;
	};
}