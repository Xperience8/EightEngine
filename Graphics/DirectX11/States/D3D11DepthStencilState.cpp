#include "stdafx.h"
#include "D3D11DepthStencilState.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDevice.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDeviceContext.h"

bool D3D11DepthStencilState::CreateApiSpecific(
	const std::shared_ptr<RenderingDevice> &pRenderingDevice)
{
	// Translate engine depth stencil state description into DirectX 11
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	depthStencilDesc.DepthEnable = m_DepthEnable;
	depthStencilDesc.DepthWriteMask = static_cast<D3D11_DEPTH_WRITE_MASK>(m_DepthWriteMask);
	depthStencilDesc.DepthFunc = static_cast<D3D11_COMPARISON_FUNC>(m_DepthFunc);
	depthStencilDesc.StencilEnable = m_StencilEnable;
	depthStencilDesc.StencilReadMask = m_StencilReadMask;
	depthStencilDesc.StencilWriteMask = m_StencilWriteMask;
	depthStencilDesc.BackFace.StencilDepthFailOp = static_cast<D3D11_STENCIL_OP>(
		m_BackFace.StencilDepthFailOp);
	depthStencilDesc.BackFace.StencilFailOp = static_cast<D3D11_STENCIL_OP>(
		m_BackFace.StencilFailOp);
	depthStencilDesc.BackFace.StencilPassOp = static_cast<D3D11_STENCIL_OP>(
		m_BackFace.StencilPassOp);
	depthStencilDesc.BackFace.StencilFunc = static_cast<D3D11_COMPARISON_FUNC>(
		m_BackFace.StencilFunc);
	depthStencilDesc.FrontFace.StencilDepthFailOp = static_cast<D3D11_STENCIL_OP>(
		m_FrontFace.StencilDepthFailOp);
	depthStencilDesc.FrontFace.StencilFailOp = static_cast<D3D11_STENCIL_OP>(
		m_FrontFace.StencilFailOp);
	depthStencilDesc.FrontFace.StencilPassOp = static_cast<D3D11_STENCIL_OP>(
		m_FrontFace.StencilPassOp);
	depthStencilDesc.FrontFace.StencilFunc = static_cast<D3D11_COMPARISON_FUNC>(
		m_FrontFace.StencilFunc);

	HRESULT hr = pRenderingDevice->GetDevice3D()->CreateDepthStencilState(
		&depthStencilDesc, &m_pDepthStencilState);

	if (FAILED(hr))
	{
		EIGHT_ERROR("Creating DirectX 11 depth stencil state failed!");
		return false;
	}

	EIGHT_LOG("DirectX 11 depth stencil state was successfully created");

	return true;
}
void D3D11DepthStencilState::BindApiSpecific(
	const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
{
	pRenderingDeviceContext->GetContext3D()->OMSetDepthStencilState(
		m_pDepthStencilState, m_StencilRef);
}