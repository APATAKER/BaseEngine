#pragma once
#include <physics/iPlaneComponent.h>
#include "btBulletDynamicsCommon.h"
#include "nConvert.h"


class cPlaneComponent : public nPhysics::iPlaneComponent
{
	friend class cPhysWorld;
public:
	virtual ~cPlaneComponent();

	// it is created from the Physics Wrapper object
	
	void GetTransform(glm::mat4& transformOut) override;
	void ApplyForce(const glm::vec3& force) override;
	void ApplyRotation(const glm::vec3& rotation) override;

	void SetPosition(const glm::vec3& pos) override;
	glm::vec3 GetPosition() override;
	void JumpForce(const glm::vec3& force) override;
	int GetNumberOfPhysicalObject() override;
	bool GetTransform(int index, glm::mat4& transformOut) override;

	
	// constructor
	cPlaneComponent(nPhysics::sPlaneDef plane_def);

private:
	btRigidBody* m_body_;

	//not to be used
	cPlaneComponent() = delete;
	cPlaneComponent(const cPlaneComponent& other) = delete;
	cPlaneComponent& operator=(const cPlaneComponent& other) = delete;

};
