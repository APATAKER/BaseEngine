#pragma once
#include "physics/iBumperComponent.h"
#include "btBulletDynamicsCommon.h"
#include "nConvert.h"

class cBumperComponent : public nPhysics::iBumperComponent
{
	friend class cPhysWorld;

public:
	virtual ~cBumperComponent();

	// it is created from the Physics Wrapper object

	void GetTransform(glm::mat4& transformOut) override;
	void ApplyForce(const glm::vec3& force) override;
	void ApplyRotation(const glm::vec3& rotation) override;

	void SetPosition(const glm::vec3& pos) override;
	glm::vec3 GetPosition() override;
	void JumpForce(const glm::vec3& force) override;
	int GetNumberOfPhysicalObject() override;
	bool GetTransform(int index, glm::mat4& transformOut) override;




	// constructor  (creates rigidbodies)
	cBumperComponent(const nPhysics::sBumperDef& bumper_def);

private:
	btRigidBody* m_body_;

	//not to be used
	cBumperComponent() = delete;
	cBumperComponent(const cBumperComponent& other) = delete;
	cBumperComponent& operator=(const cBumperComponent& other) = delete;
};