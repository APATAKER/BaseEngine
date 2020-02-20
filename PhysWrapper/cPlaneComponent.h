#pragma once
#include <physics/iPlaneComponent.h>
#include "iShape.h"
#include "cRigidBody.h"

class cPlaneComponent : public nPhysics::iPlaneComponent
{
public:
	virtual ~cPlaneComponent();

	// it is created from the Physics Wrapper object
	
	void GetTransform(glm::mat4& transformOut) override;
	void ApplyForce(const glm::vec3& force) override;
	
	// constructor
	cPlaneComponent(nPhysics::sPlaneDef plane_def);
	
	physLib::sRigidBodyDef rigid_body_def;
	physLib::cRigidBody* rigid_body;
	physLib::iShape* shape;

};
