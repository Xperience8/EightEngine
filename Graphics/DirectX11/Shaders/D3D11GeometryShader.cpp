#include "stdafx.h"
#include "D3D11GeometryShader.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDevice.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDeviceContext.h"

bool D3D11GeometryShader::CreateApiSpecific(const BYTE *pShaderBytecode, UINT bytecodeSize,
	const std::shared_ptr<RenderingDevice> &pDevice)
{
	auto &pDevice3D = pDevice->GetDevice3D();

	HRESULT hr = pDevice3D->CreateGeometryShader(pShaderBytecode, bytecodeSize,
		nullptr, &m_pGeometryShader);

	if (FAILED(hr))
	{
		EIGHT_ERROR("Creating geometry shader failed!");
		return false;
	}

	return true;
}

void D3D11GeometryShader::BindApiSpecifc(
	const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
{
	pRenderingDeviceContext->GetContext3D()->GSSetShader(m_pGeometryShader, nullptr, 0);
}