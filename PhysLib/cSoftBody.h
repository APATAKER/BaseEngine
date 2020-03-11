#pragma once

#include "glm_headers.h"
#include <vector>
#include "cCollisionBody.h"

namespace physLib
{
	struct sSoftBodyNodeDef
	{
		glm::vec3 Position;
		float Mass;
	};
	struct sSoftBodyDef
	{
		std::vector<sSoftBodyNodeDef> Nodes;
		std::vector<std::pair<size_t,size_t>> Springs;
		float SpringConstant;
		
	};

	class cSoftBody :public cCollisionBody
	{
	private:
		class cSpring;
		class cNode
		{
		public:
			cNode(const sSoftBodyNodeDef& node_def);
			void CalculateRadius();
			inline  bool isFixed() { return Mass == 0.f; }
			bool IsNeighbor(cNode* other);
			std::vector<cSpring*> Springs;
			float Radius;
			float Mass;
			glm::vec3 Position;
			glm::vec3 Velocity;
			glm::vec3 Acceleration;
			
		};
		class cSpring
		{
		public:
			cSpring(cNode* nodeA, cNode* nodeB, float springConstant);
			void UpdateSpringForce();
			void ApplyForceToNodes();
			cNode* GetOther(cNode* node);
			cNode* NodeA;
			cNode* NodeB;
			float SpringConstant;
			float RestingLength;
			glm::vec3 SpringForceAtoB;
			
		};
	public:
		cSoftBody(const sSoftBodyDef& def);
		~cSoftBody();

		void ClearAccelerations() override;
		glm::vec3 GetVelocity() override;
		glm::vec3 GetAccelerations() override;
		std::string GetAiType() override;
		void setVelocity(glm::vec3 velocity) override;
		void setAcceleration(glm::vec3 accl) override;
		
		size_t NumNodes();
		bool GetNodeRadius(size_t index, float& radiusOut);
		bool GetNodePosition(size_t index, glm::vec3& positionOut);

		

		void Intergrate(float dt, const glm::vec3& gravity);
	protected:
		void IntegrateNode(cNode* node);
		
	private:
		std::vector<cNode*> mNodes;
		std::vector<cSpring*> mSprings;

	};
}
