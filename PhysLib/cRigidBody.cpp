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
		mAT = glm::vec3(0, 0, 1);
		mUp = glm::vec3(0, 1, 0);
		mOrientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
		
	}

	cRigidBody::~cRigidBody()
	{
	}

	void cRigidBody::GetTransform(glm::mat4& transformOut)
	{
		glm::mat4 translation_transform = glm::translate(glm::mat4(1.f), this->mPosition);
		transformOut *= translation_transform;

		glm::mat4 rotation_transform = glm::mat4(this->getQOrientation());
		transformOut *= rotation_transform;
	}

	void cRigidBody::ApplyForce(const glm::vec3& force)
	{
		this->updateAtFromOrientation();
		float mag_force = force.z;
		glm::vec3 direction = this->getAtInWorldSpace() - this->mPosition;
		direction = normalize(direction);
		glm::vec3 forceToBeApplied = mag_force * direction;
		this->mAcceleration += (forceToBeApplied / this->mMass);	
	}

	void cRigidBody::ApplyRotation(const glm::vec3& rotation)
	{
		updateOrientation(rotation);
	}

	void cRigidBody::updateAtFromOrientation(void)
	{
		glm::mat4 matRotation = glm::mat4(this->mOrientation);

		glm::vec4 front = glm::vec4(glm::vec3(0, 0, 1), 1.0f);

		glm::vec4 newAt = matRotation * front;

		// Update the "At"
		this->mAT = glm::vec3(newAt);
	}

	glm::vec3 cRigidBody::getAtInWorldSpace(void)
	{
		return this->mPosition + this->mAT;
	}

	void cRigidBody::updateOrientation(glm::vec3 EulerAngleDegreesXYZ)
	{
		// Create a quaternion of this angle...
		glm::vec3 EulerAngleRadians;
		EulerAngleRadians.x = glm::radians(EulerAngleDegreesXYZ.x);
		EulerAngleRadians.y = glm::radians(EulerAngleDegreesXYZ.y);
		EulerAngleRadians.z = glm::radians(EulerAngleDegreesXYZ.z);

		glm::quat angleChange = glm::quat(EulerAngleRadians);

		// ...apply it to the exiting rotation
		this->mOrientation *= angleChange;
	}

	glm::quat cRigidBody::getQOrientation(void)
	{
		return this->mOrientation;
	}

	//constructor for RigidBodyDef
	sRigidBodyDef::sRigidBodyDef()
		:Mass(0),Velocity(glm::vec3(0,0,0)),Position(glm::vec3(0,0,0)),At(glm::vec3(0,0,1))
	{
	}
}
