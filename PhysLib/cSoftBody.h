#pragma once

#include "glm_headers.h"
#include <vector>

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

	class cSoftBody
	{
	private:
		class cSpring;
		class cNode
		{
		public:
			cNode(const sSoftBodyNodeDef& node_def);
			void CalculateRadius();
			inline  bool isFixed() { return Mass == 0.f; }
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

		
		size_t NumNodes();
		bool GetNodeRadius(size_t index, float& radiusOut);
		bool GetNodePosition(size_t index, glm::vec3& positionOut);

		//void Intergrate
	protected:
		void IntegrateNode(cNode* node);
		
	private:
		std::vector<cNode*> mNodes;
		std::vector<cSpring*> mSprings;

	};
}
