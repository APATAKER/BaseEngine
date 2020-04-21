#include "cBtFlipperComponent.h"

#include "nConvert.h"


cBtFlipperComponent::~cBtFlipperComponent()
{
}

void cBtFlipperComponent::GetTransform(glm::mat4& transformOut)
{
}

void cBtFlipperComponent::ApplyForce(const glm::vec3& force)
{
}

void cBtFlipperComponent::ApplyRotation(const glm::vec3& rotation)
{
}

void cBtFlipperComponent::SetPosition(const glm::vec3& pos)
{
}

glm::vec3 cBtFlipperComponent::GetPosition()
{
	return glm::vec3();
}

void cBtFlipperComponent::JumpForce(const glm::vec3& force)
{
}

int cBtFlipperComponent::GetNumberOfPhysicalObject()
{
	return 0;
}

bool cBtFlipperComponent::GetTransform(int index, glm::mat4& transformOut)
{
	btTransform transform;
	m_body_->getMotionState()->getWorldTransform(transform);
	nConvert::ToSimple(transform, transformOut);
	return true;
}

cBtFlipperComponent::cBtFlipperComponent(const nPhysics::sFlipperDef& Flipper_def)
	:iFlipperComponent(Flipper_def)
{
	btCollisionShape* shape = new btBoxShape(btVector3(5.f,5.f,5.f));
	btTransform transform;
	btScalar mass(Flipper_def.Mass);
	transform.setIdentity();
	transform.setOrigin(nConvert::ToBullet(Flipper_def.Position));
	btVector3 local_inertia(0, 0, 0);
	shape->calculateLocalInertia(mass, local_inertia);
}
