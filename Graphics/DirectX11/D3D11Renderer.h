#pragma once

#include "EightEngine\Graphics\EffectCache.h"
#include "EightEngine\Graphics\ConstantBufferCache.h"
#include "EightEngine\Graphics\IRenderableEntity.h"

#include "EightEngine\Graphics\ApiWrapper\RenderingDevice.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDeviceContext.h"

class Screen;
class RenderingScene;

class RendererBase
{
protected:
	RendererBase() { }
	~RendererBase() { }

	std::shared_ptr<RenderingDevice> m_pRenderingDevice;
	std::shared_ptr<RenderingDeviceContext> m_pRenderingDeviceContext;
};

class D3D11Renderer : public RendererBase
{
protected:
	D3D11Renderer() { }
	~D3D11Renderer() { }

	bool CreateDeviceApiSpecific();
};