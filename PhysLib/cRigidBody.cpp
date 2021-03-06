#include "cRigidBody.h"  // My header

#include <glm/ext/matrix_transform.inl>


namespace physLib
{
	cRigidBody::cRigidBody(const sRigidBodyDef& def, iShape* shape)
		:cCollisionBody(eBodyType::rigid)
	{
		mPosition = def.Position;
		mMass = def.Mass;
		mVelocity = def.Velocity;
		mSteerForce = glm::vec3(0,0,0);
		mAcceleration = glm::vec3(0, 0, 0);
		mInvMass = (def.Mass == 0) ? 0 : (1 / def.Mass);
		mShape = shape;
		mAT = glm::vec3(0, 0, 1);
		mUp = glm::vec3(0, 1, 0);
		glm::quat baseglm(glm::vec3(0.0f, 0.0f, 0.0f));
		mOrientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
		if (mOrientation != def.Orientation)
		{
			mOrientation = def.Orientation;
		}
		mAiType = def.AiType;
		
	}

	cRigidBody::~cRigidBody()
	{
	}

	void cRigidBody::ClearAccelerations()
	{
		mAcceleration.x = 0.f;
		mAcceleration.y = 0.f;
		mAcceleration.z = 0.f;
	}

	void cRigidBody::GetTransform(glm::mat4& transformOut)
	{
		glm::mat4 translation_transform = glm::translate(glm::mat4(1.f), this->mPosition);
		transformOut *= translation_transform;

		glm::mat4 rotation_transform = glm::mat4(this->getQOrientation());
		transformOut *= rotation_transform;
	}
	int cRigidBody::GetNumberOfPhysicalObjects()
	{
		return this->numberOfPhysicalObjects;
	}

	//void cRigidBody::ApplyForce(const glm::vec3& force)
	//{
	//	this->updateAtFromOrientation();
	//	float mag_force = force.z;
	//	glm::vec3 direction = this->getAtInWorldSpace() - this->mPosition;
	//	direction = normalize(direction);
	//	glm::vec3 forceToBeApplied = mag_force * direction;
	//	this->mAcceleration += (forceToBeApplied / this->mMass);	
	//}

	void cRigidBody::ApplyForce(const glm::vec3& force)
	{
		this->mVelocity = force;
	}

	void cRigidBody::jump(const glm::vec3& force)
	{
		this->mAcceleration += (force / this->mMass);
	}

	void cRigidBody::ApplyRotation(const glm::vec3& rotation)
	{
		updateOrientation(rotation);
	}

	void cRigidBody::SetPosition(const glm::vec3& position)
	{
		this->mPosition = position;
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

	glm::quat cRigidBody::safeQuatLookAt(glm::vec3 const& lookFrom, glm::vec3 const& lookTo, glm::vec3 const& up)
	{
			glm::vec3  direction = lookTo - lookFrom;
			float      directionLength = glm::length(direction);

			// Check if the direction is valid; Also deals with NaN
			if (!(directionLength > 0.0001))
				return glm::quat(1, 0, 0, 0); // Just return identity

			// Normalize direction
			direction /= directionLength;

			// Is the normal up (nearly) parallel to direction?
			if (glm::abs(glm::dot(direction, up)) <= .9999f)
				return glm::quatLookAt(direction, up);
			else
				return glm::quat(1, 0, 0, 0);

		
	}

	void cRigidBody::setAcceleration(glm::vec3 accl)
	{
		mAcceleration = accl;
	}

	void cRigidBody::setVelocity(glm::vec3 velocity)
	{
		mVelocity = velocity;
	}

	glm::vec3 cRigidBody::GetAccelerations()
	{
		return mAcceleration;
	}

	glm::vec3 cRigidBody::GetVelocity()
	{
		return mVelocity;
	}

	std::string cRigidBody::GetAiType()
	{
		return mAiType;
	}

	glm::vec3 cRigidBody::GetPosition()
	{
		return this->mPosition;
	}

	glm::vec3 cRigidBody::GetPreviousPosition()
	{
		return this->mPreviousPosition;
	}

	void cRigidBody::GetAabb(glm::vec3& minBoundsOut, glm::vec3& maxBoundsOut)
	{
	}

	void cRigidBody::RecalculateAABB()
	{
		this->mAabb.mMinBound = glm::vec3(FLT_MAX);
		this->mAabb.mMaxBound = glm::vec3(0);

		if(this->GetShape()->GetShapeType() == eShapeType::plane)
		{
			this->mAabb.mMinBound.x = -256.f;
			this->mAabb.mMaxBound.x = 256.f;
			this->mAabb.mMinBound.y = 0.f;
			this->mAabb.mMaxBound.y = 10.f;
			this->mAabb.mMinBound.z = -256.f;
			this->mAabb.mMaxBound.z = 256.f;
		}
		else
		{
			this->mAabb.mMinBound.x = glm::min(this->mAabb.mMinBound.x, (this->mPosition.x - 10));
			this->mAabb.mMaxBound.x = glm::max(this->mAabb.mMaxBound.x, (this->mPosition.x + 10));
			this->mAabb.mMinBound.y = glm::min(this->mAabb.mMinBound.y, (this->mPosition.y - 10));
			this->mAabb.mMaxBound.y = glm::max(this->mAabb.mMaxBound.y, (this->mPosition.y + 10));
			this->mAabb.mMinBound.z = glm::min(this->mAabb.mMinBound.z, (this->mPosition.z - 10));
			this->mAabb.mMaxBound.z = glm::max(this->mAabb.mMaxBound.z, (this->mPosition.z + 10));
		}
		
		
	}

	//constructor for RigidBodyDef
	sRigidBodyDef::sRigidBodyDef()
		:Mass(0),Velocity(glm::vec3(0,0,0)),Position(glm::vec3(0,0,0)),At(glm::vec3(0,0,1))
	{
	}
}
