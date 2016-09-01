#include "stdafx.h"
#include "RenderingPass.h"
#include "EightEngine\Graphics\IRenderableEntity.h"
#include "EightEngine\Graphics\Effect.h"
#include "EightEngine\Graphics\ApiWrapper\Renderer.h"
#include "EightEngine\GameView\GameCamera.h"
#include "FrustumCullingSystem.h"


void RenderingPass::Render(Renderer &renderer,
	const std::shared_ptr<GameCamera> &pCamera)
{
	EIGHT_ASSERT(m_pEffect, "You are trying to render but rendering pipeline is not setted");
	
	if (m_RenderableEntities.empty())
		return;

	m_pEffect->Bind(renderer.GetRenderingDeviceContext());

	for (auto &renderableEntity : m_RenderableEntities)
	{
		renderableEntity->pRenderableEntity->VRender(renderer);
	}
}

void RenderingPass::RenderDebug(Renderer &renderer)
{
	for (auto &renderableEntity : m_RenderableEntities)
	{
		renderableEntity->pRenderableEntity->VRenderDebug(renderer);
	}
}


