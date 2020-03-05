#include "cClothComponent.h"

cClothComponent::~cClothComponent()
{
	if(mBody)
	{
		delete mBody;
		mBody = 0;
	}
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
	return mBody->NumNodes();
}

bool cClothComponent::GetNodeRadius(size_t index, float& radiusout)
{
	return mBody->GetNodeRadius(index,radiusout);
}

bool cClothComponent::GetNodePosition(size_t index, glm::vec3& positionOut)
{
	return mBody->GetNodePosition(index, positionOut);
}

cClothComponent::cClothComponent(const nPhysics::sClothDef& cloth_def)
	:mBody(0)
{
	//struct sSoftBodyNodeDef
	//{
	//	glm::vec3 Position;
	//	float Mass;
	//};
	
	size_t numNodes = cloth_def.NumNodesDown * cloth_def.NumNodesAcross;
	physLib::sSoftBodyDef soft_body_def;

	soft_body_def.SpringConstant = cloth_def.SpringConstant;
	
	soft_body_def.Nodes.resize(numNodes);
	size_t idxNodes = 0;
	
	glm::vec3 sepAcross(cloth_def.CornerB - cloth_def.CornerA);
	sepAcross /= cloth_def.NumNodesAcross - 1;

	glm::vec3 sepDown(glm::normalize(cloth_def.DownDirection));
	sepDown *= glm::length(sepAcross);
	
	for (size_t idxAcross = 0; idxAcross < cloth_def.NumNodesAcross; idxAcross++)
	{
		for (size_t idxDown = 0; idxDown < cloth_def.NumNodesDown; idxDown++)
		{
			glm::vec3 position = cloth_def.CornerA + sepAcross * (float)idxAcross + sepDown * (float)idxDown;
			soft_body_def.Nodes[idxNodes].Position = position;// CALCULATE POSITION 
			soft_body_def.Nodes[idxNodes].Mass = cloth_def.NodeMass;
		}
	}

	// Set the spring infomation
	for (size_t idxAcross = 0; idxAcross < cloth_def.NumNodesAcross-1; idxAcross++)
	{
		for (size_t idxDown = 0; idxDown < cloth_def.NumNodesDown-1; idxDown++)
		{
			// set the across spring, going right
			// set the down spring, going down
		}
	}
	// set the bottom row springs
	for (size_t idxAcross = 0; idxAcross < cloth_def.NumNodesAcross-1; idxAcross++)
	{
		// set the bottom row spring going right
	}
	// set the rightmost column
	for (size_t idxDown = 0; idxDown < cloth_def.NumNodesDown-1; idxDown++)
	{
		// set the right most column spring going down
	}
	
}
