#pragma once
#include <physics/iCollisionListener.h>
#include "btBulletDynamicsCommon.h"

class cPhysCollisionListener
{
public:
	cPhysCollisionListener(nPhysics::iCollisionListener* m_collision_listener);
	virtual ~cPhysCollisionListener();
	virtual void Collide(btRigidBody* bodyA, btRigidBody* bodyB);
private:
	nPhysics::iCollisionListener* m_collision_listener_;
};
