#pragma once
#include "cPhysFactory.h"

#define DLL_EXPORT extern "C" __declspec(dllexport)

// createing physics Wrapper factory object if using DLL loading
DLL_EXPORT nPhysics::iPhysicsFactory* make_physics_factory();