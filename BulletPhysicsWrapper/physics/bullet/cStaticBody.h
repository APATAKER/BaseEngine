#pragma once
#include "physics/iStaticComponent.h"
#include "btBulletDynamicsCommon.h"
#include "nConvert.h"

class cStaticComponent : public nPhysics::iStaticComponent
{
	friend class cPhysWorld;

public:
	virtual ~cStaticComponent();

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
	cStaticComponent(const nPhysics::sStaticDef& static_def);

private:
	btRigidBody* m_body_;

	//not to be used
	cStaticComponent() = delete;
	cStaticComponent(const cStaticComponent& other) = delete;
	cStaticComponent& operator=(const cStaticComponent& other) = delete;
};
