#include "stdafx.h"
#include "UIRenderingPass.h"
#include "EightEngine\Graphics\ApiWrapper\Buffers\IndexBuffer.h"
#include "EightEngine\Graphics\Rendering\FrustumCullingSystem.h"


struct RenderCommand
{
	std::shared_ptr<Texture2D> pTexture;
	UINT VertexStart;
	UINT VertexCount;

	float Depth;
};





bool UIRenderingPass::Init(Renderer &renderer)
{
	m_TextVertexBuffer.SetProperties(UsageFlag::USAGE_DYNAMIC, CpuAccessFlag::CPU_ACCESS_WRITE);
	if (!m_TextVertexBuffer.Create(nullptr, m_MaxTextSize * 4, sizeof(PosColTexVertex ),
		renderer.GetRenderingDevice()))
	{
		return false;
	}

	m_SpriteVertexBuffer.SetProperties(UsageFlag::USAGE_DYNAMIC, CpuAccessFlag::CPU_ACCESS_WRITE);
	if (!m_SpriteVertexBuffer.Create(nullptr, m_MaxSpriteSize * 4, sizeof(PosTexVertex),
		renderer.GetRenderingDevice()))
	{
		return false;
	}

	m_QuadVertexBuffer.SetProperties(UsageFlag::USAGE_DYNAMIC, CpuAccessFlag::CPU_ACCESS_WRITE);
	if (!m_QuadVertexBuffer.Create(nullptr, m_MaxQuadSize * 4, sizeof(PosColVertex),
		renderer.GetRenderingDevice()))
	{
		return false;
	}

	std::vector<USHORT> indices(
		m_MaxSpriteSize > m_MaxTextSize ? m_MaxSpriteSize : m_MaxTextSize * 6);
	for (USHORT i = 0, k = 0; i < indices.size(); i += 6, k += 4)
	{
		indices[i] = k;
		indices[i + 1] = k + 1;
		indices[i + 2] = k + 2;
		indices[i + 3] = k;
		indices[i + 4] = k + 2;
		indices[i + 5] = k + 3;
	}

	m_IndexBuffer.SetProperties(UsageFlag::USAGE_IMMUTABLE, CpuAccessFlag::NO_CPU_ACCESS);
	if (!m_IndexBuffer.Create(reinterpret_cast<const BYTE*>(indices.data()), 
		static_cast<UINT>(indices.size()), sizeof(USHORT), renderer.GetRenderingDevice()))
	{
		return false;
	}

	return true;
}
void UIRenderingPass::Render(Renderer &renderer,
	const std::shared_ptr<GameCamera> &pCamera)
{
	auto &pRenderingDeviceContext = renderer.GetRenderingDeviceContext();

	if (m_RenderableEntities.empty())
		return;

	for (auto &renderableEntity : m_RenderableEntities)
	{
		auto pMenuRenderingComponent = static_cast<UIRenderComponent*>(
			renderableEntity->pRenderableEntity);

		pMenuRenderingComponent->GetUserInterface()->VRender(*this);
	}

	m_IndexBuffer.Bind(pRenderingDeviceContext);


	// Rendering Background quads
	if (m_QuadBatch.GetVertexCount() > 0)
	{
		renderer.GetEffect("UserInterfaceWithoutTexturesEffect")->Bind(pRenderingDeviceContext);
		m_QuadVertexBuffer.Bind(pRenderingDeviceContext);

		m_QuadVertexBuffer.UpdateVertices(reinterpret_cast<const BYTE*>(m_QuadBatch.GetVertices()),
			m_QuadBatch.GetVertexCount(), pRenderingDeviceContext);

		pRenderingDeviceContext->DrawIndexed(
			static_cast<UINT>(m_QuadBatch.GetVertexCount() * 1.5f), 0, 0);
	}









	std::vector<RenderCommand> renderingCommands;
	for (auto &spriteBatch : m_SpriteBatchingMap)
	{
		PosTexVertex *pVertices = reinterpret_cast<PosTexVertex*>(spriteBatch.second.GetVertices());
		for (UINT i = 0; i < spriteBatch.second.GetVertexCount(); i += 4)
		{
			RenderCommand command;
			command.pTexture = spriteBatch.first;
			command.Depth = pVertices[i].Position.z;
			command.VertexCount = 6;
			command.VertexStart = static_cast<UINT>(i * 1.5);

			renderingCommands.push_back(command);
		}
	}

	std::sort(renderingCommands.begin(), renderingCommands.end(), 
		[](const RenderCommand &r1, const RenderCommand &r2) {
		return r1.Depth > r2.Depth;
	});
	

	// Rendering Background images
	renderer.GetEffect("UserInterfaceEffect")->Bind(pRenderingDeviceContext);

	m_SpriteVertexBuffer.Bind(pRenderingDeviceContext);
	
	UINT lastTextureId = 0;

	for (auto &command : renderingCommands)
	{
		auto &pTexture = command.pTexture;

		if (reinterpret_cast<UINT>(pTexture.get()) != lastTextureId)
		{
			lastTextureId = reinterpret_cast<UINT>(pTexture.get());

			auto spriteBatch = m_SpriteBatchingMap[pTexture];
			m_SpriteVertexBuffer.UpdateVertices(
				reinterpret_cast<const BYTE*>(spriteBatch.GetVertices()),
				spriteBatch.GetVertexCount(), pRenderingDeviceContext);

			pTexture->BatchBind(pRenderingDeviceContext);
			pRenderingDeviceContext->BatchBindShaderResourceViewPS(0);
		}

		pRenderingDeviceContext->DrawIndexed(command.VertexCount, command.VertexStart, 0);
	}











	/*
	// Rendering Background images
	renderer.GetEffect("UserInterfaceEffect")->Bind(pRenderingDeviceContext);
	
	m_SpriteVertexBuffer.Bind(pRenderingDeviceContext);
	for (auto &spriteBatch : m_SpriteBatchingMap)
	{
		auto &pTexture = spriteBatch.first;

		m_SpriteVertexBuffer.UpdateVertices(
			reinterpret_cast<const BYTE*>(spriteBatch.second.GetVertices()),
			spriteBatch.second.GetVertexCount(), pRenderingDeviceContext);

		pTexture->BatchBind(pRenderingDeviceContext);
		pRenderingDeviceContext->BatchBindShaderResourceViewPS(0);

		pRenderingDeviceContext->DrawIndexed(
			static_cast<UINT>(spriteBatch.second.GetVertexCount() * 1.5f), 0, 0);
	}
	*/

	// Rendering Text
	m_pEffect->Bind(pRenderingDeviceContext);
	m_TextVertexBuffer.Bind(pRenderingDeviceContext);
	
	for (auto &textBatch : m_TextBatchingMap)
	{
		auto &pTexture = textBatch.first->GetTexture();
		auto &textureDesc = pTexture->GetTextureDesc();

		m_TextVertexBuffer.UpdateVertices(reinterpret_cast<const BYTE*>(
			textBatch.second.GetVertices()), textBatch.second.GetVertexCount(),
			pRenderingDeviceContext);

		TextCBData cbData(
			static_cast<float>(textureDesc.TextureWidth),
			static_cast<float>(textureDesc.TextureHeight),
			static_cast<float>(renderer.GetScreen()->GetScreenWidth()),
			static_cast<float>(renderer.GetScreen()->GetScreenHeight())
			);
		pRenderingDeviceContext->GetConstantBuffer(TextCBData::NAME)->UpdateBuffer(
			&cbData, pRenderingDeviceContext);

		pTexture->BatchBind(pRenderingDeviceContext);
		pRenderingDeviceContext->BatchBindShaderResourceViewPS(0);

		pRenderingDeviceContext->DrawIndexed(
			static_cast<UINT>(textBatch.second.GetVertexCount() * 1.5f), 0, 0);
	}

	m_TextBatchingMap.clear();
	m_SpriteBatchingMap.clear();
	m_QuadBatch.Reset();
}
