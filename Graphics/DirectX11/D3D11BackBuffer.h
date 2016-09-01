#pragma once

#include "D3D11RenderTarget.h"

class D3D11BackBuffer
{
private:
	D3D11RenderTarget m_RenderTarget;

	UINT m_VsyncLevel;

	IDXGISwapChain *m_pSwapChain;

	bool CreateSwapChain(const XmlNode &configNode, HWND windowHandle,
		const std::shared_ptr<RenderingDevice> &pRenderingDevice);
public:
	D3D11BackBuffer() : m_pSwapChain(nullptr), m_VsyncLevel(0) { }
	~D3D11BackBuffer()
	{
		if (m_pSwapChain)
		{
			m_pSwapChain->SetFullscreenState(false, nullptr);
			D3D11_SAFE_RELEASE(m_pSwapChain);
		}
	}

	void Destroy()
	{
		if (m_pSwapChain)
		{
			m_pSwapChain->SetFullscreenState(false, nullptr);
			D3D11_SAFE_RELEASE(m_pSwapChain);
		}
		m_RenderTarget.Destroy();
	}

	bool Init(const XmlNode &configNode, HWND windowHandle,
		const std::shared_ptr<RenderingDevice> &pRenderingDevice);

	bool Resize(UINT resX, UINT resY,
		const std::shared_ptr<RenderingDevice> &pRenderingDevice);

	void Present()
	{
		m_pSwapChain->Present(m_VsyncLevel, 0);
	}

	D3D11RenderTarget &GetRenderTarget() { return m_RenderTarget; }
};

typedef D3D11BackBuffer BackBuffer;
