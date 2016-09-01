#include "stdafx.h"
#include "D3D11VertexShader.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDevice.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDeviceContext.h"

bool D3D11VertexShader::CreateApiSpecific(const BYTE *pShaderBytecode, UINT bytecodeSize,
	const std::shared_ptr<RenderingDevice> &pDevice)
{
	auto &pDevice3D = pDevice->GetDevice3D();

	HRESULT hr = pDevice3D->CreateVertexShader(pShaderBytecode, bytecodeSize,
		nullptr, &m_pVertexShader);

	if (FAILED(hr))
	{
		EIGHT_ERROR("Creating vertex shader failed!");
		return false;
	}

	return true;
}
void D3D11VertexShader::BindApiSpecifc(
	const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
{
	pRenderingDeviceContext->GetContext3D()->VSSetShader(m_pVertexShader, nullptr, 0);
}