#pragma once
#include <string>

#include "glm_headers.h"
#include "iShape.h"     // iShape base class for all shapes
#include "cCollisionBody.h"

namespace physLib
{
	// sRigidBodyDef
	// 
	// Contains all non-shape related information
	// necessary to create a cRigidBody instance.
	struct sRigidBodyDef  
	{

		//friend class cWorld;
		//       initialize everything to reasonable values.
		// Constructor:
		sRigidBodyDef();

		

		// default values are set to be the rigidbody is static, and pos and vel to be 0.f

		// Mass
		// Must be set to a non-negative value.
		// 0.f will result in a static rigid body.
		// A positive value will result in a dynamic rigid body.
		float Mass;
		// Position
		// The initial position of the rigid body.
		glm::vec3 Position;
		// Velocity
		// The initial linear velocity of the rigid body.
		glm::vec3 Velocity;
		// The initaial AT vector(facing direction)
		glm::vec3 At;
		// The Ai type
		std::string AiType;
		// Orientation
		glm::quat Orientation;
		
	};

	// cRigidBody
	//
	// Contains all values required for a rigid body simulation.
	// 
	// Shapes may be shared between rigid bodies.
	// Does not own the iShape* used to create it.
	// Will not delete the iShape* it contains when it is deleted.
	class cRigidBody :public cCollisionBody
	{
		// cWorld will be operating on cRigidBody values quite a bit
		// We will trust it to do everything correctly.
		friend class cWorld;
		friend class cAI;
		friend class cCoordinator;
	public:
		// Constructor
		// All internal values are initialized according to the sRigidBodyDef
		// A valid iShape* is expected.
		cRigidBody(const sRigidBodyDef& def, iShape* shape);



		// Destructor
		// Will not delete the contained iShape.
		virtual ~cRigidBody();

		void ClearAccelerations() override;
		
		// GetTransform
		// Retrieve a rendering-friendly form of position + rotation
		void GetTransform(glm::mat4& transformOut);

		// GetNumberOfPhysicalObjects
		// Retrieve number of objects to render
		int GetNumberOfPhysicalObjects();

		// ApplyForce
		// Apply an external force to this rigid body.
		// The force is applied center-mass, affecting the acceleration.
		void ApplyForce(const glm::vec3& force);

		void ApplyRotation(const glm::vec3& rotation);

		// TODO:
		// ApplyImpulse
		// Apply an impulse to the center-mass of this rigid body.
		// ApplyImpulse(const glm::vec3& impulse);

		// IsStatic
		// A static rigid body was created with a mass of 0.f.
		// A static rigid body does react to other rigid bodies in the world.
		inline bool IsStatic() { return mMass == 0.f; }

		// GetShape
		// Get this rigid body's shape.
		// Each rigid body must be created with a shape.
		inline iShape* GetShape() { return mShape; }

		// GetShapeType
		// Get the type of shape of this rigid body's shape.
		// Equivalent to GetShape()->GetShapeType()
		inline eShapeType GetShapeType() { return mShape->GetShapeType(); }

	private:

		void* mUserPointer;
		// My shape, expected to be valid.
		// cRigidBody will not delete mShape in its destructor.
		iShape* mShape;

		// During a timestep, the previous position is stored for collision related purposes.
		glm::vec3 mPreviousPosition;
		// Current linear position.
		glm::vec3 mPosition;
		// Current linear velocity.
		glm::vec3 mVelocity;
		// Current linear acceleration.
		glm::vec3 mAcceleration;
		// Looking direction
		glm::vec3 mAT;
		// Up Vector
		glm::vec3 mUp;
		// Orientation
		glm::quat mOrientation;
		// Ai type
		std::string mAiType;
		// Steering Force
		glm::vec3 mSteerForce;
		// NumberOfPhysicalObjects
		int numberOfPhysicalObjects = 1;
		// Orientation Functions
		void updateAtFromOrientation(void);
		glm::vec3 getAtInWorldSpace(void);
		void updateOrientation(glm::vec3 EulerAngleDegreesXYZ);
		glm::quat getQOrientation(void);

		glm::quat safeQuatLookAt(glm::vec3 const& lookFrom, glm::vec3 const& lookTo, glm::vec3 const& up);

		void setAcceleration(glm::vec3 accl) override;
		void setVelocity(glm::vec3 velocity) override;
		glm::vec3 GetAccelerations() override;
		glm::vec3 GetVelocity() override;
		std::string GetAiType() override;
		
		// Mass
		// Expected to be non-negative.
		// A value of 0 infers a static rigid body.
		float mMass;
		// Inverse mass
		// Stored for convenience.
		// If mMass is 0, mInvMass will also be 0.
		float mInvMass;


		// Constructors not to be used.
		cRigidBody() = delete;
		cRigidBody(const cRigidBody& other) = delete;
		cRigidBody& operator=(const cRigidBody& other) = delete;
	};
}