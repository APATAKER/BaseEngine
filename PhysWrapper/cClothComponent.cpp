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

int cClothComponent::GetNumberOfPhysicalObject()
{
	return (int)mBody->NumNodes();
	//return 0;
}

bool cClothComponent::GetTransform(int index, glm::mat4& transformOut)
{
	mBody->GetTransform(index, transformOut);
	return true;
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
	// Set the node infomation
	
	for (size_t idxAcross = 0; idxAcross < cloth_def.NumNodesAcross; idxAcross++)
	{
		for (size_t idxDown = 0; idxDown < cloth_def.NumNodesDown; idxDown++)
		{
			glm::vec3 position = cloth_def.CornerA + sepAcross * (float)idxAcross + sepDown * (float)idxDown;
			soft_body_def.Nodes[idxNodes].Position = position;// CALCULATE POSITION 
			soft_body_def.Nodes[idxNodes].Mass = cloth_def.NodeMass;

			idxNodes++;
		}
	}
	// SET the top row node massed to 0
	for (size_t idxAcross = 0; idxAcross < numNodes; idxAcross += cloth_def.NumNodesAcross)
	{
		soft_body_def.Nodes[idxAcross].Mass = 0.f;
	}
	
	// Set the spring infomation
	for (size_t idxAcross = 0; idxAcross < cloth_def.NumNodesAcross-1; idxAcross++)
	{
		for (size_t idxDown = 0; idxDown < cloth_def.NumNodesDown-1; idxDown++)
		{
			idxNodes = (idxDown * cloth_def.NumNodesAcross) + idxAcross;
			// set the across spring, going right
			soft_body_def.Springs.push_back(std::make_pair(idxNodes, idxNodes + 1));
			
			// set the down spring, going down
			soft_body_def.Springs.push_back(std::make_pair(idxNodes, idxNodes + cloth_def.NumNodesAcross));
		}
	}
	// set the bottom row springs
	size_t idxBottomRowStart = cloth_def.NumNodesAcross * (cloth_def.NumNodesDown - 1);
	for (size_t idxAcross = 0; idxAcross < cloth_def.NumNodesAcross-1; idxAcross++)
	{
		// set the bottom row spring going right
		soft_body_def.Springs.push_back(std::make_pair(idxBottomRowStart + idxAcross, idxBottomRowStart + idxAcross + 1));
		
	}
	// set the rightmost column
	for (size_t idxDown = 0; idxDown < cloth_def.NumNodesDown-1; idxDown++)
	{
		idxNodes = cloth_def.NumNodesAcross * (idxDown + 1) - 1;
		// set the right most column spring going down
		soft_body_def.Springs.push_back(std::make_pair(idxNodes , idxNodes + cloth_def.NumNodesAcross));
		
	}
	//for(size_t idx = 0;idx<soft_body_def.Springs.size();idx++)
	//{
	//	
	//}
	
	mBody = new physLib::cSoftBody(soft_body_def);
	
}
