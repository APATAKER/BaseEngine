#pragma once
#include <physics/iBallComponent.h>
#include "iShape.h"
#include "cRigidBody.h"

class cBallComponent : public nPhysics::iBallComponent
{
public:
	virtual ~cBallComponent();

	// it is created from the Physics Wrapper object

	void GetTransform(glm::mat4& transformOut) override;
	void ApplyForce(const glm::vec3& force) override;
	

	// constructor  (creates rigidbodies)
	cBallComponent(const nPhysics::sBallDef& ball_def);

	physLib::sRigidBodyDef rigid_body_def;
	physLib::cRigidBody* rigid_body;
	physLib::iShape* shape;
	


};
