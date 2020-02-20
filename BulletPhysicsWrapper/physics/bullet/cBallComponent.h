#pragma once
#include <physics/iBallComponent.h>
#include "btBulletDynamicsCommon.h"
#include "nConvert.h"

class cBallComponent : public nPhysics::iBallComponent
{
	friend class cPhysWorld;
public:
	virtual ~cBallComponent();

	// it is created from the Physics Wrapper object

	void GetTransform(glm::mat4& transformOut) override;
	void ApplyForce(const glm::vec3& force) override;
	

	// constructor  (creates rigidbodies)
	cBallComponent(const nPhysics::sBallDef& ball_def);

private:
	btRigidBody* m_body_;

	//not to be used
	cBallComponent() = delete;
	cBallComponent(const cBallComponent& other) = delete;
	cBallComponent& operator=(const cBallComponent& other) = delete;
	


};
