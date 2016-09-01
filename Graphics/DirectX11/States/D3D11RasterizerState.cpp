#include "stdafx.h"
#include "D3D11RasterizerState.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDevice.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDeviceContext.h"

bool D3D11RasterizerState::CreateApiSpecific(
	const std::shared_ptr<RenderingDevice> &pRenderingDevice)
{
	// Translate engine description to DirectX 11
	D3D11_RASTERIZER_DESC rasterizerDesc;
	rasterizerDesc.FillMode = static_cast<D3D11_FILL_MODE>(m_FillMode);
	rasterizerDesc.CullMode = static_cast<D3D11_CULL_MODE>(m_CullMode);
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.f;
	rasterizerDesc.SlopeScaledDepthBias = 0.f;

	HRESULT hr = pRenderingDevice->GetDevice3D()->CreateRasterizerState(&rasterizerDesc,
		&m_pRasterizerState);
	
	if (FAILED(hr))
	{
		EIGHT_ERROR("Creating rasterizer state failed");
		return false;
	}

	EIGHT_LOG("DirectX 11 rasterizer state was successfully created");

	return true;
}
void D3D11RasterizerState::BindApiSpecific(
	const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
{
	pRenderingDeviceContext->GetContext3D()->RSSetState(m_pRasterizerState);
}