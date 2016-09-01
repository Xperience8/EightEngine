#include "stdafx.h"
#include "D3D11ComputeShader.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDevice.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDeviceContext.h"

bool D3D11ComputeShader::CreateApiSpecific(const BYTE *pShaderBytecode, UINT bytecodeSize,
	const std::shared_ptr<RenderingDevice> &pDevice)
{
	auto &pDevice3D = pDevice->GetDevice3D();

	HRESULT hr = pDevice3D->CreateComputeShader(pShaderBytecode, bytecodeSize,
		nullptr, &m_pComputeShader);

	if (FAILED(hr))
	{
		EIGHT_ERROR("Creating compute shader failed!");
		return false;
	}

	return true;
}

void D3D11ComputeShader::BindApiSpecifc(
	const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
{
	pRenderingDeviceContext->GetContext3D()->CSSetShader(m_pComputeShader, nullptr, 0);
}