#pragma once

#include "EightEngine\Graphics\ApiWrapper\RenderingDevice.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDeviceContext.h"

class D3D11RenderTarget
{
public:
	D3D11RenderTarget() : m_pRenderTargetView(nullptr), m_pDepthStencilView(nullptr) { }
	~D3D11RenderTarget()
	{
		D3D11_SAFE_RELEASE(m_pRenderTargetView);
		D3D11_SAFE_RELEASE(m_pDepthStencilView);
	}

	void Destroy()
	{
		D3D11_SAFE_RELEASE(m_pRenderTargetView);
		D3D11_SAFE_RELEASE(m_pDepthStencilView);
	}

	// Create normal render target with specific format and specific size
	bool Create(UINT width, UINT height, FormatFlag format,
		const std::shared_ptr<RenderingDevice> &pRenderingDevice);

	// Create render target as back buffer
	bool Create(IDXGISwapChain *pSwapChain,
		const std::shared_ptr<RenderingDevice> &pRenderingDevice);

	// Clear render target with black color, 
	// clear depth buffer with 1.f and stencil buffer with 0
	void Clear(const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
	{
		pRenderingDeviceContext->GetContext3D()->ClearRenderTargetView(
			m_pRenderTargetView, DirectX::Colors::Black);
		pRenderingDeviceContext->GetContext3D()->ClearDepthStencilView(
			m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}

	// Push render target into renderer
	void Apply(const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
	{
		pRenderingDeviceContext->GetContext3D()->OMSetRenderTargets(
			1, &m_pRenderTargetView, m_pDepthStencilView);
		pRenderingDeviceContext->GetContext3D()->RSSetViewports(1, &m_Viewport);
	}
private:
	CD3D11_VIEWPORT m_Viewport;
	ID3D11RenderTargetView *m_pRenderTargetView;
	ID3D11DepthStencilView *m_pDepthStencilView;
};

typedef D3D11RenderTarget RenderTarget;
