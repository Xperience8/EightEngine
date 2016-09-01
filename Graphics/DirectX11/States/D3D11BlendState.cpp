#include "stdafx.h"
#include "D3D11BlendState.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDevice.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDeviceContext.h"

bool D3D11BlendState::CreateApiSpecific(
	const std::shared_ptr<RenderingDevice> &pRenderingDevice)
{
	// translate engine blend description to DirectX 11
	D3D11_BLEND_DESC blendDesc;
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = m_RenderTargetWriteMask;
	blendDesc.RenderTarget[0].SrcBlend = static_cast<D3D11_BLEND>(m_SourceBlend);
	blendDesc.RenderTarget[0].DestBlend = static_cast<D3D11_BLEND>(m_DestinationBlend);
	blendDesc.RenderTarget[0].BlendOp = static_cast<D3D11_BLEND_OP>(m_BlendOp);
	blendDesc.RenderTarget[0].SrcBlendAlpha = static_cast<D3D11_BLEND>(m_SourceBlendAlpha);
	blendDesc.RenderTarget[0].DestBlendAlpha = static_cast<D3D11_BLEND>(m_DestinationBlendAlpha);
	blendDesc.RenderTarget[0].BlendOpAlpha = static_cast<D3D11_BLEND_OP>(m_BlendOpAlpha);

	HRESULT hr = pRenderingDevice->GetDevice3D()->CreateBlendState(&blendDesc, &m_pBlendState);

	if (FAILED(hr))
	{
		EIGHT_ERROR("Creating DirectX 11 blend state failed");
		return false;
	}

	EIGHT_LOG("DirectX 11 blend state was successfully created");

	return true;
}
void D3D11BlendState::BindApiSpecific(const std::shared_ptr<RenderingDeviceContext> &
	pRenderingDeviceContext)
{
	pRenderingDeviceContext->GetContext3D()->OMSetBlendState(m_pBlendState,
		reinterpret_cast<const float*>(&m_BlendFactor), m_SampleMask);
}