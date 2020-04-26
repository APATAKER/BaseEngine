#pragma once
#include <physics/iPhysicsWorld.h>
#include <vector>
#include "cBallComponent.h">
#include "cPlaneComponent.h">
#include "cBtFlipperComponent.h"
#include "cStaticBody.h"
#include "cPhysCollisionListener.h"



class cPhysWorld : public nPhysics::iPhysicsWorld
{
	friend class cBtFlipperComponent;
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

	void SetUpAi() override;
	void GetOnPath(bool onPath) override;
	void GetIsFlock(bool is_flock) override;
	// Getting is_Reverse or not
	void GetIsReverse(bool isReserve) override;
	void GetFormationType(int type) override;

	bool GetkeyPressed(int obj) override;
	bool SetkeyPressed(bool keyPressed, int obj) override;

	

	// not used
	void SetCollisionListener(nPhysics::iCollisionListener* collision_listener);


	// constraints
	void hingeConstraint(btRigidBody* body, btVector3 pivot, btVector3 axis);
	void hingeConstraint(btRigidBody* body1, btVector3 pivot1, btVector3 axis1, btRigidBody* body2, btVector3 pivot2, btVector3 axis2);


	std::vector<nPhysics::iPhysicsComponent*> vp_phy_components;
	// cons-tor  
	cPhysWorld();

private:
	bool AddRigidBodies(cBallComponent* component);
	bool AddRigidBodies(cPlaneComponent* component);
	bool AddRigidBodies(cBtFlipperComponent* component);
	bool AddRigidBodies(cStaticComponent* component);
	bool AddHingeConstraint(cBtFlipperComponent* component);

	btDefaultCollisionConfiguration* m_collision_configuration;
	btCollisionDispatcher* m_collision_dispatcher;
	btBroadphaseInterface* m_overlapping_pair_cache;
	btSequentialImpulseConstraintSolver* m_solver;
	btDiscreteDynamicsWorld* m_dynamics_world_;

	
	cPhysCollisionListener* m_collision_listener_;

	char input = ' ';
	bool input_L_ = false;
	bool input_R_ = false;
	
	

	cPhysWorld(const cPhysWorld& other) = delete;
	cPhysWorld& operator=(const cPhysWorld& other) = delete;

};
