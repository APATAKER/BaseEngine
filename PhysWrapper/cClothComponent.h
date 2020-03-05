#pragma once
#include <physics/iClothComponent.h>
#include "iShape.h"
#include "cRigidBody.h"
//#include "cSoftBody.h"

class cClothComponent : public nPhysics::iClothComponent
{
public:
	virtual ~cClothComponent();

	// it is created from the Physics Wrapper object

	void GetTransform(glm::mat4& transformOut) override;
	void ApplyForce(const glm::vec3& force) override;
	void ApplyRotation(const glm::vec3& rotation) override;

	size_t NumNodes()override;
	bool GetNodeRadius(size_t index, float& radiusout) override;
	bool GetNodePosition(size_t index, glm::vec3& positionOut) override;



	// constructor  (creates rigidbodies)
	cClothComponent(const nPhysics::sClothDef& cloth_def);

	//physLib::cSoftBody* soft_body;
	physLib::sRigidBodyDef rigid_body_def;
	physLib::cRigidBody* rigid_body;
	physLib::iShape* shape;



};
