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
		virtual void SetUpAi() = 0;
		virtual void GetIsFlock(bool is_flock) = 0;
		virtual void GetIsReverse(bool isReserve) = 0;
		virtual void GetOnPath(bool onPath) = 0;
		virtual void GetFormationType(int type) = 0;
		virtual bool AddComponent(iPhysicsComponent* component) = 0;
		virtual bool RemoveComponent(iPhysicsComponent* component) = 0;

		virtual bool GetkeyPressed(int obj) = 0;
		virtual bool SetkeyPressed(bool keyPressed, int obj) = 0;

		//virtual void SetCollisionListener(nPhysics::iCollisionListener* collision_listener) = 0;
	};
}