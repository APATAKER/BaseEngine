#include "cPhysFactory.h"
#include <physics/iPhysicsFactory.h>
#include "cPhysWorld.h"
#include "cBallComponent.h"
#include "cPlaneComponent.h"
#include "cBtFlipperComponent.h"
#include "cStaticBody.h"
#include "cBumperComponent.h"



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
nPhysics::iClothComponent* cPhysFactory::CreateCloth(const nPhysics::sClothDef& def)
{
	return nullptr;
}

nPhysics::iFlipperComponent* cPhysFactory::CreateFlipper(const nPhysics::sFlipperDef& def)
{
	return new cBtFlipperComponent(def);
}

nPhysics::iStaticComponent* cPhysFactory::CreateFixed(const nPhysics::sStaticDef& def)
{
	return new cStaticComponent(def);
}

nPhysics::iBumperComponent* cPhysFactory::CreateBumper(const nPhysics::sBumperDef& def)
{
	return new cBumperComponent(def);
}
