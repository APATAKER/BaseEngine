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

void cBtFlipperComponent::GoToAngle(float angle,float dt,float target_velocity)
{
	m_constraint_->enableMotor(true);
	m_constraint_->setLimit(-1.f, 1.f);
	m_constraint_->setMotorTarget(angle,dt);
	m_constraint_->setMaxMotorImpulse(2000.f);
	m_constraint_->setMotorTargetVelocity(target_velocity);
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
	return 1;
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
	m_half_length = Flipper_def.half_length;
	btCollisionShape* shape = new btBoxShape(btVector3(nConvert::ToBullet(Flipper_def.half_length)));
	btTransform transform;
	btScalar mass(Flipper_def.Mass);
	transform.setIdentity();
	transform.setOrigin(nConvert::ToBullet(Flipper_def.Position));
	btVector3 local_inertia(0, 0, 0);
	shape->calculateLocalInertia(mass, local_inertia);
	btDefaultMotionState* motion_state = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rigid_body_construction_info(mass, motion_state, shape, local_inertia);
	m_body_ = new btRigidBody(rigid_body_construction_info);
	m_body_->setUserPointer(this);

}
