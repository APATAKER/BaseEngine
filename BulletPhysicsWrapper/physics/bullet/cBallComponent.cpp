#include "cBallComponent.h"
#include <glm/ext/matrix_transform.inl>

cBallComponent::~cBallComponent()
{
	m_body_->setUserPointer(0);
	delete m_body_->getMotionState();
	delete m_body_;
	m_body_ = 0;
}

void cBallComponent::GetTransform(glm::mat4& transformOut)
{
	btTransform transform;
	m_body_->getMotionState()->getWorldTransform(transform);
	nConvert::ToSimple(transform, transformOut);
}

void cBallComponent::ApplyForce(const glm::vec3& force)
{
	m_body_->activate(true);
	m_body_->applyCentralForce(nConvert::ToBullet(force));
}


cBallComponent::cBallComponent(const nPhysics::sBallDef& ball_def)
	:iBallComponent(ball_def)
{
	btCollisionShape* shape = new btSphereShape(ball_def.Radius);
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(nConvert::ToBullet(ball_def.Position));

	btScalar mass(ball_def.Mass);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool is_dynamic = (mass != 0.f);

	btVector3 local_inertia(0, 0, 0);
	if (is_dynamic)
		shape->calculateLocalInertia(mass, local_inertia);

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* motion_state = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rigid_body_construction_info(mass, motion_state, shape, local_inertia);
	rigid_body_construction_info.m_restitution = 0.5f;
	m_body_ = new btRigidBody(rigid_body_construction_info);
	m_body_->setUserPointer(this);
}



