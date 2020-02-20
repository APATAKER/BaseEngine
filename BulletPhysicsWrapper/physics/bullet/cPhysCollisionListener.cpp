#include "cPhysCollisionListener.h"
#include <iostream>

cPhysCollisionListener::cPhysCollisionListener(nPhysics::iCollisionListener* m_collision_listener):m_collision_listener_(m_collision_listener)
{
}

cPhysCollisionListener::~cPhysCollisionListener()
{
	m_collision_listener_ = 0;
}

void cPhysCollisionListener::Collide(btRigidBody* bodyA, btRigidBody* bodyB)
{
	if(bodyA || bodyB)
	{
		std::cout << "Received null body in collision Listener" << std::endl;
	}
	m_collision_listener_->Collide(reinterpret_cast<nPhysics::iPhysicsComponent*>(bodyA->getUserPointer()),
								   reinterpret_cast<nPhysics::iPhysicsComponent*>(bodyB->getUserPointer()));
}

