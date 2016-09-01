#include "stdafx.h"
#include "D3D11DomainShader.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDevice.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDeviceContext.h"

bool D3D11DomainShader::CreateApiSpecific(const BYTE *pShaderBytecode, UINT bytecodeSize,
	const std::shared_ptr<RenderingDevice> &pDevice)
{
	auto &pDevice3D = pDevice->GetDevice3D();

	HRESULT hr = pDevice3D->CreateDomainShader(pShaderBytecode, bytecodeSize,
		nullptr, &m_pDomainShader);

	if (FAILED(hr))
	{
		EIGHT_ERROR("Creating domain shader failed!");
		return false;
	}

	return true;
}

void D3D11DomainShader::BindApiSpecifc(
	const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
{
	pRenderingDeviceContext->GetContext3D()->DSSetShader(m_pDomainShader, nullptr, 0);
}