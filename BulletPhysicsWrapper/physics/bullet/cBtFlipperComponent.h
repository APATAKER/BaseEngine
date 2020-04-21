#pragma once
#include "physics/iFlipperComponent.h"
#include "btBulletDynamicsCommon.h"


class cBtFlipperComponent : public nPhysics::iFlipperComponent
{
public:
	virtual ~cBtFlipperComponent();

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
	cBtFlipperComponent(const nPhysics::sFlipperDef& Flipper_def);

private:
	btRigidBody* m_body_;
	//not to be used
	cBtFlipperComponent() = delete;
	cBtFlipperComponent(const cBtFlipperComponent& other) = delete;
	cBtFlipperComponent& operator=(const cBtFlipperComponent& other) = delete;
};
