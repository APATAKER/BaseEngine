#include "cBumperComponent.h"

cBumperComponent::cBumperComponent(const nPhysics::sBumperDef& bumper_def)
	:iBumperComponent(bumper_def)
{
	btCollisionShape* shape = new btCylinderShapeZ(nConvert::ToBullet(bumper_def.half_length));
	btTransform transform;
	btScalar mass(0);
	transform.setIdentity();
	transform.setOrigin(nConvert::ToBullet(bumper_def.Position));
	btVector3 local_inertia(0, 0, 0);
	shape->calculateLocalInertia(mass, local_inertia);
	btDefaultMotionState* motion_state = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rigid_body_construction_info(mass, motion_state, shape, local_inertia);
	rigid_body_construction_info.m_restitution = 5.f;
	m_body_ = new btRigidBody(rigid_body_construction_info);
	m_body_->setUserPointer(this);
}

cBumperComponent::~cBumperComponent()
{
}

void cBumperComponent::GetTransform(glm::mat4& transformOut)
{
}

void cBumperComponent::ApplyForce(const glm::vec3& force)
{
}

void cBumperComponent::ApplyRotation(const glm::vec3& rotation)
{
}

void cBumperComponent::SetPosition(const glm::vec3& pos)
{
}

glm::vec3 cBumperComponent::GetPosition()
{
	return glm::vec3();
}

void cBumperComponent::JumpForce(const glm::vec3& force)
{
}

int cBumperComponent::GetNumberOfPhysicalObject()
{
	return 1;
}

bool cBumperComponent::GetTransform(int index, glm::mat4& transformOut)
{
	btTransform transform;
	m_body_->getMotionState()->getWorldTransform(transform);
	nConvert::ToSimple(transform, transformOut);
	return true;
}
