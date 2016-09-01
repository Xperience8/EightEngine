#include "stdafx.h"
#include "D3D11PixelShader.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDevice.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDeviceContext.h"

bool D3D11PixelShader::CreateApiSpecific(const BYTE *pShaderBytecode, UINT bytecodeSize,
	const std::shared_ptr<RenderingDevice> &pDevice)
{
	auto &pDevice3D = pDevice->GetDevice3D();

	HRESULT hr = pDevice3D->CreatePixelShader(pShaderBytecode, bytecodeSize,
		nullptr, &m_pPixelShader);

	if (FAILED(hr))
	{
		EIGHT_ERROR("Creating pixel shader failed!");
		return false;
	}

	return true;
}

void D3D11PixelShader::BindApiSpecifc(
	const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
{
	pRenderingDeviceContext->GetContext3D()->PSSetShader(m_pPixelShader, nullptr, 0);
}