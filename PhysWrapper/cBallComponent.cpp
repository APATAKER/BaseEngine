#include "cBallComponent.h"
//#include <glm/ext/matrix_transform.inl>
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
void cBallComponent::JumpForce(const glm::vec3& force)
{
	rigid_body->jump(force);
}
void cBallComponent::SetPosition(const glm::vec3& pos)
{
	rigid_body->SetPosition(pos);
}
glm::vec3 cBallComponent::GetPosition()
{
	return rigid_body->GetPosition();
}
void cBallComponent::ApplyRotation(const glm::vec3& rotation)
{
	rigid_body->ApplyRotation(rotation);
}

int cBallComponent::GetNumberOfPhysicalObject()
{
	return rigid_body->GetNumberOfPhysicalObjects();
}

bool cBallComponent::GetTransform(int index, glm::mat4& transformOut)
{
	rigid_body->GetTransform(transformOut);
	return true;
}



cBallComponent::cBallComponent(const nPhysics::sBallDef& ball_def)
	:iBallComponent(ball_def)
{
	shape = new  physLib::cSphere(ball_def.Radius);
	rigid_body_def.Position = ball_def.Position;
	rigid_body_def.Mass = ball_def.Mass;
	rigid_body_def.Velocity = glm::vec3(0, 0, 0);
	rigid_body_def.AiType = ball_def.AiType;
	rigid_body_def.Orientation = ball_def.orientation;
	rigid_body = new physLib::cRigidBody(rigid_body_def,shape);
}



