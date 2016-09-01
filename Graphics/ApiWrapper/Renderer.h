#pragma once

#include "EightEngine\Graphics\EffectCache.h"
#include "EightEngine\Graphics\RenderingStatesCache.h"
#include "EightEngine\Graphics\ConstantBufferCache.h"
#include "EightEngine\Graphics\IRenderableEntity.h"

#include "RenderingDevice.h"
#include "RenderingDeviceContext.h"

#include "EightEngine\Graphics\DirectX11\D3D11Renderer.h"
#include "EightEngine\Graphics\DirectX11\D3D11BackBuffer.h"

class Screen;
class ResourceCache;
class ConstantBufferCache;
class XmlNode;
class RenderingScene;
class GameCamera;
#include "EightEngine\EventManager\IEventData.h"

class Renderer : public D3D11Renderer
{
private:
	BackBuffer m_BackBuffer;
	RenderTarget *m_pRenderTarget;

	std::shared_ptr<Screen> m_pScreen;
	std::shared_ptr<RenderingScene> m_pRenderingScene;

	std::shared_ptr<EffectCache> m_pEffectCache;
	std::shared_ptr<RenderingStatesCache> m_pRenderingStatesCache;


	std::vector<std::shared_ptr<GameCamera> > m_CameraList;

	bool m_DebugRenderingStatus;

	void ActivateDebugRendering(const EventPtr<IEventData> &pEvent);
public:
	Renderer() : m_DebugRenderingStatus(false), m_pRenderTarget(nullptr) { }
	~Renderer() { }

	bool StartUp(const XmlNode &configNode, const std::shared_ptr<Screen> &pScreen);
	void ShutDown();

	bool OnResize(UINT ResX, UINT ResY);

	void Render();

	void PushCamera(const std::shared_ptr<GameCamera> &pCamera)
	{
		m_CameraList.push_back(pCamera);
	}

	void SetBackBufferAsRenderTarget()
	{
		m_BackBuffer.GetRenderTarget().Clear(m_pRenderingDeviceContext);
		m_BackBuffer.GetRenderTarget().Apply(m_pRenderingDeviceContext);
	}
	void Present()
	{
		m_BackBuffer.Present();
	}


	bool IsDebugRenderingOn() const { return m_DebugRenderingStatus; }

	void SetScreen(const std::shared_ptr<Screen> &pScreen)
	{
		m_pScreen = pScreen;
	}

	const std::shared_ptr<Screen> &GetScreen() const
	{
		return m_pScreen;
	}
	const std::shared_ptr<Effect> &GetEffect(const std::string &effectName) const
	{
		return m_pEffectCache->GetEffect(effectName);
	}
	const std::shared_ptr<RenderingDevice> &GetRenderingDevice() const
	{
		return m_pRenderingDevice;
	}
	const std::shared_ptr<RenderingDeviceContext> &GetRenderingDeviceContext() const
	{
		return m_pRenderingDeviceContext;
	}

	UINT Picking(UINT scX, UINT scY, const std::shared_ptr<GameCamera> &pCamera);
};

extern Renderer g_Renderer;