#include "cPlaneComponent.h"
#include <glm/ext/matrix_transform.inl>
#include "shapes.h"

cPlaneComponent::~cPlaneComponent()
{
	delete rigid_body;
	delete shape;
}

void cPlaneComponent::GetTransform(glm::mat4& transformOut)
{
	rigid_body->GetTransform(transformOut);
}

void cPlaneComponent::ApplyForce(const glm::vec3& force)
{
	rigid_body->ApplyForce(force);
}


cPlaneComponent::cPlaneComponent(nPhysics::sPlaneDef plane_def)
	:iPlaneComponent(plane_def)
{
	plane_def.Constant = glm::dot(plane_def.Normal, plane_def.position);
	shape = new physLib::cPlane(plane_def.Normal, plane_def.Constant);
	rigid_body_def.Position = plane_def.position;
	rigid_body_def.Velocity = glm::vec3(0, 0, 0);
	rigid_body_def.Mass = 0;

	rigid_body = new physLib::cRigidBody(rigid_body_def, shape);
}
