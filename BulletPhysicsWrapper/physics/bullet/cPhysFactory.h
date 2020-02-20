#pragma once
#include <physics/iPhysicsFactory.h>

class cPhysFactory : public nPhysics::iPhysicsFactory
{
public:
	virtual ~cPhysFactory();

	// World and Components are created using this. used at PhysicsInit()
	nPhysics::iPhysicsWorld* CreateWorld() override;
	// components
	nPhysics::iBallComponent* CreateBall(const nPhysics::sBallDef& def) override;
	nPhysics::iPlaneComponent* CreatePlane(const nPhysics::sPlaneDef& def) override;
};