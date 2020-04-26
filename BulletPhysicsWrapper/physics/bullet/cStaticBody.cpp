#include "cStaticBody.h"

cStaticComponent::cStaticComponent(const nPhysics::sStaticDef& static_def)
	:iStaticComponent(static_def)
{
	btCollisionShape* shape = new btBoxShape(btVector3(nConvert::ToBullet(static_def.half_length)));
	btTransform transform;
	btScalar mass(0);
	transform.setIdentity();
	transform.setOrigin(nConvert::ToBullet(static_def.Position));
	btVector3 local_inertia(0, 0, 0);
	shape->calculateLocalInertia(mass, local_inertia);
	btDefaultMotionState* motion_state = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rigid_body_construction_info(mass, motion_state, shape, local_inertia);
	m_body_ = new btRigidBody(rigid_body_construction_info);
	m_body_->setUserPointer(this);
}

cStaticComponent::~cStaticComponent()
{
}

void cStaticComponent::GetTransform(glm::mat4& transformOut)
{
}

void cStaticComponent::ApplyForce(const glm::vec3& force)
{
}

void cStaticComponent::ApplyRotation(const glm::vec3& rotation)
{
}

void cStaticComponent::SetPosition(const glm::vec3& pos)
{
}

glm::vec3 cStaticComponent::GetPosition()
{
	return glm::vec3();
}

void cStaticComponent::JumpForce(const glm::vec3& force)
{
}

int cStaticComponent::GetNumberOfPhysicalObject()
{
	return 1;
}

bool cStaticComponent::GetTransform(int index, glm::mat4& transformOut)
{
	btTransform transform;
	m_body_->getMotionState()->getWorldTransform(transform);
	nConvert::ToSimple(transform, transformOut);
	return true;
}
