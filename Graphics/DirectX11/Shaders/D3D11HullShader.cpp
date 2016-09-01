#include "stdafx.h"
#include "D3D11HullShader.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDevice.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDeviceContext.h"

bool D3D11HullShader::CreateApiSpecific(const BYTE *pShaderBytecode, UINT bytecodeSize,
	const std::shared_ptr<RenderingDevice> &pDevice)
{
	auto &pDevice3D = pDevice->GetDevice3D();

	HRESULT hr = pDevice3D->CreateHullShader(pShaderBytecode, bytecodeSize,
		nullptr, &m_pHullShader);

	if (FAILED(hr))
	{
		EIGHT_ERROR("Creating hull shader failed!");
		return false;
	}

	return true;
}

void D3D11HullShader::BindApiSpecifc(
	const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
{
	pRenderingDeviceContext->GetContext3D()->HSSetShader(m_pHullShader, nullptr, 0);
}