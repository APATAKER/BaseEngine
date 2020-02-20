#pragma once
#include <glm/mat4x4.hpp>

#include "physics/iDebugRenderer.h"
#include "btBulletDynamicsCommon.h"

class cPhysDebugRenderer :public nPhysics::iDebugRenderer
{
public:
	cPhysDebugRenderer(nPhysics::iDebugRenderer* debug_renderer);
	virtual ~cPhysDebugRenderer();
	virtual void DrawSphere(const glm::mat4 transform, float radius);
private:
	nPhysics::iDebugRenderer* m_debug_renderer_;
};