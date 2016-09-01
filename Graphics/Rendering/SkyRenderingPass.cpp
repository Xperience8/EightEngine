#include "stdafx.h"
#include "SkyRenderingPass.h"
#include "EightEngine\Graphics\IRenderableEntity.h"
#include "EightEngine\Actor\Components\SkyRenderComponent.h"
#include "EightEngine\Graphics\Effect.h"
#include "EightEngine\Graphics\ApiWrapper\Renderer.h"
#include "EightEngine\Actor\Components\TransformComponent.h"
#include "EightEngine\Graphics\Rendering\FrustumCullingSystem.h"
#include "EightEngine\GameView\GameCamera.h"

void SkyRenderingPass::Render(Renderer &renderer,
	const std::shared_ptr<GameCamera> &pCamera)
{
	EIGHT_ASSERT(m_pEffect, "You are trying to render but rendering pipeline is not setted");
	if (m_RenderableEntities.empty())
		return;

	m_pEffect->Bind(renderer.GetRenderingDeviceContext());

	for (auto &entity : m_RenderableEntities)
	{
		DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixTranslationFromVector(
			pCamera->GetPosition());
		DirectX::XMMATRIX worldViewProjMatrix = DirectX::XMMatrixMultiply(
			worldMatrix, pCamera->GetViewProjMatrix());
		entity->pTransform->SetWorldMatrix(worldViewProjMatrix);

		entity->pRenderableEntity->VRender(renderer);
	}
}