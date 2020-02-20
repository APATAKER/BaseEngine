#pragma once
#include <physics/iPhysicsWorld.h>
#include <vector>
#include <phys.h>
#include "cBallComponent.h">
#include "cPlaneComponent.h">
#include "cPhysCollisionListener.h"



class cPhysWorld : public nPhysics::iPhysicsWorld
{
public:
	// des-tor
	virtual ~cPhysWorld();

	// updates the physicsComponents by calling Physics Library Functions
	void Update(float dt) override;
	// converts the physicsComponents(is inside Physics Wrapper) to cRigidbody(is inside Physics Library)
	// and Add the rigid-bodies into the vector<cRigidbodies>mBodies which is inside cWorld(Physics Library)
	bool AddComponent(nPhysics::iPhysicsComponent* component) override;

	// remove duplicate stuff
	bool RemoveComponent(nPhysics::iPhysicsComponent* component) override;

	// not used
	void SetCollisionListener(nPhysics::iCollisionListener* collision_listener);


	// cons-tor  
	cPhysWorld();

private:
	bool AddRigidBodies(cBallComponent* component);
	bool AddRigidBodies(cPlaneComponent* component);
	physLib::cWorld* world_;
	cPhysCollisionListener* m_collision_listener_;
	

	

	std::vector<nPhysics::iPhysicsComponent*> vp_phy_components;
};
