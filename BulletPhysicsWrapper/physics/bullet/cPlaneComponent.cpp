#include "cPlaneComponent.h"
#include <glm/ext/matrix_transform.inl>

cPlaneComponent::~cPlaneComponent()
{
	m_body_->setUserPointer(0);
	delete m_body_->getCollisionShape();
	delete m_body_->getMotionState();
	delete m_body_;
}

void cPlaneComponent::GetTransform(glm::mat4& transformOut)
{
	btTransform transform;
	m_body_->getMotionState()->getWorldTransform(transform);
	nConvert::ToSimple(transform, transformOut);
}

void cPlaneComponent::ApplyForce(const glm::vec3& force)
{
}


cPlaneComponent::cPlaneComponent(nPhysics::sPlaneDef plane_def)
	:iPlaneComponent(plane_def)
{
	plane_def.Constant = glm::dot(plane_def.Normal, plane_def.position);
	btCollisionShape* shape = new btStaticPlaneShape(nConvert::ToBullet(plane_def.Normal),plane_def.Constant);
	btTransform transform;
	transform.setIdentity();
	//transform.setOrigin(nConvert::ToBullet(plane_def.position));

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* motion_state = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rigid_body_construction_info(0.f, motion_state, shape, btVector3(0,0,0));
	rigid_body_construction_info.m_restitution = 1.f;
	m_body_ = new btRigidBody(rigid_body_construction_info);
	m_body_->setUserPointer(this);
}
