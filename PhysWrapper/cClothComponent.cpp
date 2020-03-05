#include "cClothComponent.h"

cClothComponent::~cClothComponent()
{
	// delete body
	// delete mshape
}

void cClothComponent::GetTransform(glm::mat4& transformOut)
{
	transformOut = glm::mat4(1.f);
}

void cClothComponent::ApplyForce(const glm::vec3& force)
{
}

void cClothComponent::ApplyRotation(const glm::vec3& rotation)
{
}

size_t cClothComponent::NumNodes()
{
	return size_t();
}

bool cClothComponent::GetNodeRadius(size_t index, float& radiusout)
{
	return false;
}

bool cClothComponent::GetNodePosition(size_t index, glm::vec3& positionOut)
{
	return false;
}

cClothComponent::cClothComponent(const nPhysics::sClothDef& cloth_def)
{
	 
}
