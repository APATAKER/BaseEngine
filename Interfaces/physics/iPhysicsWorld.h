#pragma once

#include "iPhysicsComponent.h"
#include "iCollisionListener.h"
namespace nPhysics
{

	class iPhysicsWorld
	{
	public:
		virtual ~iPhysicsWorld() {}
		
		virtual void Update(float dt) = 0;
		virtual bool AddComponent(iPhysicsComponent* component) = 0;
		virtual bool RemoveComponent(iPhysicsComponent* component) = 0;
		//virtual void SetCollisionListener(nPhysics::iCollisionListener* collision_listener) = 0;
	};
}