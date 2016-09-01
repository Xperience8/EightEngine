#include "stdafx.h"
#include "D3D11RenderTarget.h"

bool D3D11RenderTarget::Create(UINT width, UINT height, FormatFlag format,
	const std::shared_ptr<RenderingDevice> &pRenderingDevice)
{
	D3D11_SAFE_RELEASE(m_pRenderTargetView);
	D3D11_SAFE_RELEASE(m_pDepthStencilView);

	auto pDevice3D = pRenderingDevice->GetDevice3D();

	CD3D11_TEXTURE2D_DESC rtvTextureDesc(static_cast<DXGI_FORMAT>(format),
		width, height, 1, 1, D3D11_BIND_RENDER_TARGET);
	ID3D11Texture2D *pRTVTexture;
	ThrowIfFailed(pDevice3D->CreateTexture2D(&rtvTextureDesc, nullptr, &pRTVTexture));
	ThrowIfFailed(pDevice3D->CreateRenderTargetView(pRTVTexture, nullptr, &m_pRenderTargetView));

	CD3D11_TEXTURE2D_DESC dsvTextureDesc(DXGI_FORMAT_D24_UNORM_S8_UINT,
		width, height, 1, 1, D3D11_BIND_DEPTH_STENCIL);
	ID3D11Texture2D *pDSVTexture;
	ThrowIfFailed(pDevice3D->CreateTexture2D(&dsvTextureDesc, nullptr, &pDSVTexture));
	ThrowIfFailed(pDevice3D->CreateDepthStencilView(pDSVTexture, nullptr, &m_pDepthStencilView));

	m_Viewport = CD3D11_VIEWPORT(0.f, 0.f, static_cast<float>(width), static_cast<float>(height));

	pRTVTexture->Release();
	pDSVTexture->Release();

	return true;
}
bool D3D11RenderTarget::Create(IDXGISwapChain *pSwapChain,
	const std::shared_ptr<RenderingDevice> &pRenderingDevice)
{
	EIGHT_ASSERT(pSwapChain, "Back buffer can not be created because swap chain object is empty");

	D3D11_SAFE_RELEASE(m_pRenderTargetView);
	D3D11_SAFE_RELEASE(m_pDepthStencilView);

	auto pDevice3D = pRenderingDevice->GetDevice3D();

	ID3D11Texture2D *pRTVTexture;
	ThrowIfFailed(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&pRTVTexture)));
	ThrowIfFailed(pDevice3D->CreateRenderTargetView(pRTVTexture, nullptr, &m_pRenderTargetView));
	CD3D11_TEXTURE2D_DESC rtvTextureDesc;
	pRTVTexture->GetDesc(&rtvTextureDesc);


	CD3D11_TEXTURE2D_DESC dsvTextureDesc(DXGI_FORMAT_D24_UNORM_S8_UINT,
		rtvTextureDesc.Width, rtvTextureDesc.Height, 1, 1, D3D11_BIND_DEPTH_STENCIL);
	ID3D11Texture2D *pDSVTexture;
	ThrowIfFailed(pDevice3D->CreateTexture2D(&dsvTextureDesc, nullptr, &pDSVTexture));
	ThrowIfFailed(pDevice3D->CreateDepthStencilView(pDSVTexture, nullptr, &m_pDepthStencilView));

	m_Viewport = CD3D11_VIEWPORT(0.f, 0.f, static_cast<float>(rtvTextureDesc.Width),
		static_cast<float>(rtvTextureDesc.Height));

	pRTVTexture->Release();
	pDSVTexture->Release();

	return true;
}