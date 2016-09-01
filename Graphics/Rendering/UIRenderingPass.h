#pragma once

#include "RenderingPass.h"

#include "EightEngine\Graphics\ApiWrapper\Buffers\VertexBuffer.h"
#include "EightEngine\Graphics\ApiWrapper\Buffers\IndexBuffer.h"
#include "EightEngine\GameAppLayer\Screen.h"
#include "EightEngine\Graphics\UserInterface\FontSheet.h"
#include "EightEngine\ResourceCache\ResourceCache.h"
#include "EightEngine\Graphics\ApiWrapper\Renderer.h"

#include "EightEngine\Actor\Components\UIRenderComponent.h"

#include "EightEngine\Graphics\RenderingBatch.h"

typedef RenderingBatch<PosColTexVertex> TextBatch;
typedef RenderingBatch<PosTexVertex> SpriteBatch;
typedef RenderingBatch<PosColVertex> QuadBatch;

class UIRenderingPass : public RenderingPass
{
private:
	std::unordered_map<std::shared_ptr<FontSheet>, TextBatch> m_TextBatchingMap;
	std::unordered_map<std::shared_ptr<Texture2D>, SpriteBatch> m_SpriteBatchingMap;

	QuadBatch m_QuadBatch;

	VertexBuffer m_TextVertexBuffer;
	VertexBuffer m_SpriteVertexBuffer;
	VertexBuffer m_QuadVertexBuffer;

	IndexBuffer m_IndexBuffer;
	
	const UINT m_MaxTextSize;
	const UINT m_MaxSpriteSize;
	const UINT m_MaxQuadSize;
public:
	UIRenderingPass() : m_MaxTextSize(100), m_MaxSpriteSize(100), m_MaxQuadSize(100) { }
	~UIRenderingPass() { }

	bool Init(Renderer &renderer);

	void Render(Renderer &renderer,
		const std::shared_ptr<GameCamera> &pCamera);

	TextBatch &GetTextBatch(const std::shared_ptr<FontSheet> &pFont) 
	{ 
		return m_TextBatchingMap[pFont]; 
	}
	SpriteBatch &GetSpriteBatch(const std::shared_ptr<Texture2D> &pTexture)
	{
		return m_SpriteBatchingMap[pTexture];
	}
	QuadBatch &GetQuadBatch()
	{
		return m_QuadBatch;
	}
};