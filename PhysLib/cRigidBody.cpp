#include "cRigidBody.h"  // My header
#include <glm/ext/matrix_transform.inl>

namespace physLib
{
	cRigidBody::cRigidBody(const sRigidBodyDef& def, iShape* shape)
	{
		mPosition = def.Position;
		mMass = def.Mass;
		mVelocity = def.Velocity;
		mAcceleration = glm::vec3(0, 0, 0);
		mInvMass = (def.Mass == 0) ? 0 : (1 / def.Mass);
		mShape = shape;
	}

	cRigidBody::~cRigidBody()
	{
	}

	void cRigidBody::GetTransform(glm::mat4& transformOut)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.f), this->mPosition);
		transformOut *= transform;
	}

	void cRigidBody::ApplyForce(const glm::vec3& force)
	{
		this->mAcceleration += (force / this->mMass);	
	}

	//constructor for RigidBodyDef
	sRigidBodyDef::sRigidBodyDef()
		:Mass(0),Velocity(glm::vec3(0,0,0)),Position(glm::vec3(0,0,0))
	{
	}
}
