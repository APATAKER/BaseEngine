#pragma once
#include "iPhysicsComponent.h"

namespace nPhysics
{
	struct sClothDef
	{
		glm::vec3 CornerA;
		glm::vec3 CornerB;
		glm::vec3 DownDirection;
		size_t NumNodesAcross;
		size_t NumNodesDown;
		float NodeMass;
		float SpringConstant;
		
	};
	
	class iClothComponent : public iPhysicsComponent
	{
	public:
		virtual ~iClothComponent() {}

		virtual size_t NumNodes() = 0;
		virtual bool GetNodeRadius(size_t index, float& radiusout) = 0;
		virtual bool GetNodePosition(size_t index, glm::vec3& positionOut) = 0;


	protected:
		iClothComponent() :iPhysicsComponent(eComponentType::CLOTH) {}
	private:
		iClothComponent(const iClothComponent& other) = delete;
		iClothComponent& operator =(const iClothComponent& other) = delete;
	};
}
