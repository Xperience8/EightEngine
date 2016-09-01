#include "stdafx.h"
#include "D3D11SamplerState.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDevice.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDeviceContext.h"

bool D3D11SamplerState::CreateApiSpecific(
	const std::shared_ptr<RenderingDevice> &pRenderingDevice)
{
	// Translate engine sampler description do DirectX 11
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = static_cast<D3D11_FILTER>(m_FilterType);
	samplerDesc.AddressU = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(m_TextureAddressMode);
	samplerDesc.AddressV = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(m_TextureAddressMode);
	samplerDesc.AddressW = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(m_TextureAddressMode);
	samplerDesc.MipLODBias = 0.f;
	samplerDesc.MaxAnisotropy = m_MaxAnisotropy;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 0.f;
	samplerDesc.BorderColor[1] = 0.f;
	samplerDesc.BorderColor[2] = 0.f;
	samplerDesc.BorderColor[3] = 0.f;
	samplerDesc.MaxLOD = FLT_MAX;
	samplerDesc.MinLOD = -FLT_MAX;

	HRESULT hr = pRenderingDevice->GetDevice3D()->CreateSamplerState(
		&samplerDesc, &m_pSamplerState);

	if (FAILED(hr))
	{
		return false;
	}

	return true;
}
void D3D11SamplerState::BatchBindApiSpecific(
	const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
{
	pRenderingDeviceContext->BindSamplerStateInBatch(m_pSamplerState);
}