#include "cPhysDebugRenderer.h"

cPhysDebugRenderer::cPhysDebugRenderer(nPhysics::iDebugRenderer* debug_renderer)
	:m_debug_renderer_(debug_renderer)
{
}

cPhysDebugRenderer::~cPhysDebugRenderer()
{
	m_debug_renderer_ = 0;
}

void cPhysDebugRenderer::DrawSphere(const glm::mat4 transform, float radius)
{
	
}
