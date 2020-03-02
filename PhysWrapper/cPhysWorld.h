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

	// Setting up AI
	void SetUpAi() override;

	// Getting Formation Type
	void GetFormationType(int type) override;

	// Getting on_path or not
	void GetOnPath(bool onPath) override;

	// Getting is_Reverse or not
	void GetIsReverse(bool isReserve) override;

	// Getting is flock
	void GetIsFlock(bool is_flock) override;

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
