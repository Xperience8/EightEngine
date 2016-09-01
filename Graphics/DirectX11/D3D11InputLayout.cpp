#include "stdafx.h"
#include "D3D11InputLayout.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDevice.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDeviceContext.h"

bool D3D11InputLayout::CreateApiSpecific(const void *pShaderBytecode, UINT bytecodeSize,
	const std::shared_ptr<RenderingDevice> &pRenderingDevice)
{
	// Translate engine input layout description to DirectX 11
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc(m_InputLayoutDesc.size());
	for (UINT i = 0; i < m_InputLayoutDesc.size(); i++)
	{
		TranslateEngineDesc(m_InputLayoutDesc[i], inputLayoutDesc[i]);
	}

	HRESULT hr = pRenderingDevice->GetDevice3D()->CreateInputLayout(
		inputLayoutDesc.data(), static_cast<UINT>(inputLayoutDesc.size()),
		pShaderBytecode, static_cast<size_t>(bytecodeSize), &m_pInputLayout);

	if (FAILED(hr))
	{
		EIGHT_ERROR("Creating DirectX 11 input layout failed");
		return false;
	}

	EIGHT_LOG("DirectX 11 input layout was successfully created!");

	return true;
}
void D3D11InputLayout::BindApiSpecific(
	const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
{
	pRenderingDeviceContext->GetContext3D()->IASetInputLayout(m_pInputLayout);
}
void D3D11InputLayout::TranslateEngineDesc(const InputElementDesc &inputElementDesc,
	D3D11_INPUT_ELEMENT_DESC &inputElementDescDirectX)
{
	inputElementDescDirectX.SemanticName = inputElementDesc.SemanticName.c_str();
	inputElementDescDirectX.SemanticIndex = inputElementDesc.SemanticIndex;
	inputElementDescDirectX.Format = static_cast<DXGI_FORMAT>(inputElementDesc.Format);
	inputElementDescDirectX.InputSlot = inputElementDesc.InputSlot;
	inputElementDescDirectX.AlignedByteOffset = inputElementDesc.AlignedByteOffset;
	inputElementDescDirectX.InputSlotClass = static_cast<D3D11_INPUT_CLASSIFICATION>(
		inputElementDesc.InputSlotClass);
	inputElementDescDirectX.InstanceDataStepRate = inputElementDesc.InstanceDataStepRate;
}