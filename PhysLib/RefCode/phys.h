#pragma once
// A single include header for everything
// 
// Also a handy place to put a general comments
// and information about this library. For example: 
// - What this library is
// - Who created this library
// - Dependencies of this library (ahem... I mean, glm)
// - Why this library was created
// - The general approach to creating this library
//     - Are we really going for super efficiency?
//     - Perhaps we're just going for understandability and learning..?
// - Notes on usage
// - Notes about any good-to-know factos or gotchas, perhaps tidbits like:
//       "cWorld will never delete a cRigidBody"
//       "shapes can be shared between multiple cRigidBody instances"
//
// All the files required to make full use of this library:
#include "eShapeType.h"  // enum class for all supported shape types
#include "iShape.h"      // iShape is the base interface class for all shapes
#include "cRigidBody.h"  // cRigidBody is this library's rigid body representation
#include "shapes.h"      // Concrete classes for all supported shape types
#include "cWorld.h"      // cWorld is the primary container and operator of a simulation