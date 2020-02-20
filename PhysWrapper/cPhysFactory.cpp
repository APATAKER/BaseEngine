#include "cPhysFactory.h"
#include <physics/iPhysicsFactory.h>
#include "cPhysWorld.h"
#include "cBallComponent.h"
#include "cPlaneComponent.h"



cPhysFactory::~cPhysFactory()
{
}



nPhysics::iPhysicsWorld* cPhysFactory::CreateWorld()
{
	return new cPhysWorld();
}

nPhysics::iBallComponent* cPhysFactory::CreateBall(const nPhysics::sBallDef& def)
{
	return new cBallComponent(def);
}

nPhysics::iPlaneComponent* cPhysFactory::CreatePlane(const nPhysics::sPlaneDef& def)
{
	return new cPlaneComponent(def);
}
