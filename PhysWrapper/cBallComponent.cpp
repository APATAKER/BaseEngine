#include "cBallComponent.h"
#include <glm/ext/matrix_transform.inl>
#include "shapes.h"

cBallComponent::~cBallComponent()
{
	delete shape;
	delete rigid_body;
}

void cBallComponent::GetTransform(glm::mat4& transformOut)
{
	rigid_body->GetTransform(transformOut);
}

void cBallComponent::ApplyForce(const glm::vec3& force)
{
	rigid_body->ApplyForce(force);
}


cBallComponent::cBallComponent(const nPhysics::sBallDef& ball_def)
	:iBallComponent(ball_def)
{
	shape = new  physLib::cSphere(ball_def.Radius);
	rigid_body_def.Position = ball_def.Position;
	rigid_body_def.Mass = ball_def.Mass;
	rigid_body_def.Velocity = glm::vec3(0, 0, 0);
	rigid_body = new physLib::cRigidBody(rigid_body_def,shape);
}



