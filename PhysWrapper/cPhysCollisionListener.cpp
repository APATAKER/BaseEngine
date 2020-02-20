#include "cPhysCollisionListener.h"
#include <iostream>

cPhysCollisionListener::cPhysCollisionListener(nPhysics::iCollisionListener* m_collision_listener):m_collision_listener_(m_collision_listener)
{
}

cPhysCollisionListener::~cPhysCollisionListener()
{
}

void cPhysCollisionListener::Collide(physLib::cRigidBody* compA, physLib::cRigidBody* compB)
{
	if(!compA || !compB)
	{
		std::cout << "collision received null objects" << std::endl;
	}
	m_collision_listener_->Collide(reinterpret_cast<nPhysics::iPhysicsComponent*>(compA), reinterpret_cast<nPhysics::iPhysicsComponent*>(compB));
}
