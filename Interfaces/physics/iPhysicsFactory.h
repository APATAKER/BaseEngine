#pragma once
#include "iBallComponent.h"
#include "iPlaneComponent.h"
#include "iPhysicsWorld.h"
#include "iClothComponent.h"
#include "iFlipperComponent.h"
#include "iStaticComponent.h"
#include "iBumperComponent.h"

namespace nPhysics
{
	class iPhysicsFactory
	{
	public:
		virtual ~iPhysicsFactory() {}
		virtual iPhysicsWorld* CreateWorld() = 0;
		// components
		virtual iBallComponent* CreateBall(const sBallDef& def) = 0;
		virtual iPlaneComponent* CreatePlane(const sPlaneDef& def) = 0;
		virtual iClothComponent* CreateCloth(const sClothDef& def) = 0;
		virtual iFlipperComponent* CreateFlipper(const sFlipperDef& def) = 0;
		virtual iStaticComponent* CreateFixed(const sStaticDef& def) = 0;
		virtual iBumperComponent* CreateBumper(const sBumperDef& def) = 0;
	};
}