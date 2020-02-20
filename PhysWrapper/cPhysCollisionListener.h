#pragma once
#include <physics/iCollisionListener.h>
#include "cRigidBody.h"

class cPhysCollisionListener
{
public:
	cPhysCollisionListener(nPhysics::iCollisionListener* m_collision_listener);
	virtual ~cPhysCollisionListener();
	virtual void Collide(physLib::cRigidBody* compA, physLib::cRigidBody* compB);

private:
	nPhysics::iCollisionListener* m_collision_listener_;
};
