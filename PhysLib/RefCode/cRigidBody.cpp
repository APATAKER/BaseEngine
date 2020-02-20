#include "cRigidBody.h"  // My header

namespace phys
{
	cRigidBody::cRigidBody(const sRigidBodyDef& def, iShape* shape)
	{
		// TODO: Initialize all internal values according 'def' and 'shape'
	}

	cRigidBody::~cRigidBody()
	{
		// TODO: Is there any clean-up required here?
	}

	void cRigidBody::GetTransform(glm::mat4& transformOut)
	{
		// TODO: Populate transformOut with correct values.
	}

	void cRigidBody::ApplyForce(const glm::vec3& force)
	{
		// TODO: Apply the force to this rigid body's acceleration
		//       Remember... F=ma  ...so an applied acceleration will be f/m
	}
}
