#include "makeFactory.h"

DLL_EXPORT nPhysics::iPhysicsFactory* make_physics_factory()
{
	// initizlie the Physics Wrapper Factory
	return new cPhysFactory();
}
