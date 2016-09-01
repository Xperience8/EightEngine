#pragma once

#include "EightEngine\Graphics\DirectX11\D3D11InputLayout.h"
#include "RenderingDeviceContext.h"

class InputLayout : public D3D11InputLayout
{
public:
	InputLayout() { }
	~InputLayout() { }

	// Creating input layout from vertex shader bytecode
	// You must first insert at least one input element with AddInputElement call
	// Parameters:
	// pShaderBytecode -> pointer to shader's bytecode, it can not be null
	// bytecodeSize -> size of shader's bytecode, it can not be zero
	// pRenderingDevice -> smart pointer to rendering device, it can not be null
	bool Create(const void *pShaderBytecode, UINT bytecodeSize,
		const std::shared_ptr<RenderingDevice> &pRenderingDevice)
	{
		EIGHT_ASSERT(!m_InputLayoutDesc.empty(),
			"You must first insert element description before calling Create");
		EIGHT_ASSERT(pShaderBytecode,
			"Pointer to shader bytecode can not be null");
		EIGHT_ASSERT(bytecodeSize,
			"Shader's bytecode size can not be zero");

		return CreateApiSpecific(pShaderBytecode, bytecodeSize, pRenderingDevice);
	}
	// Binding input layout into input assembler stage
	// If this input layout is allready binded then this call is skipped
	void Bind(const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
	{
		if (pRenderingDeviceContext->GetRenderingPipelineTable().TryBindInputLayout(
			GetAddress()))
		{
			BindApiSpecific(pRenderingDeviceContext);
		}
	}
};