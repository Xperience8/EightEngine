#pragma once

#include "EightEngine\Graphics\DirectX11\Shaders\D3D11DomainShader.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDeviceContext.h"
#include "EightEngine\Graphics\ApiWrapper\States\SamplerState.h"
#include "EightEngine\Graphics\ApiWrapper\Buffers\ConstantBuffer.h"

class DomainShader : public D3D11DomainShader
{
public:
	DomainShader() { }
	~DomainShader() { }

	// Creating domain shader from memory and creating batching group
	// with constant buffers and with sampler states
	// Registering binding function with batching group
	// Parameters:
	// pShaderBytecode -> pointer to shader bytecode, it can not be null
	// byteCodeSize -> size of shader bytecode, it can not be zero
	// pRenderingDevice -> smart pointer to rendering device, can not be null
	//					-> it is used for creating shaders
	// pRenderingDeviceContext -> smart pointer to rendering device context, can not be null
	//						   -> it is used for registering binding functions inside batching groups
	// This function is virtual
	bool VCreate(const BYTE *pShaderBytecode, UINT byteCodeSize,
		const std::shared_ptr<RenderingDevice> &pRenderingDevice,
		const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext) override
	{
		// Checked if data exist and if their size isn't zero
		EIGHT_ASSERT(pShaderBytecode,
			"You can not pass empty pointer to shader bytecode");
		EIGHT_ASSERT(byteCodeSize > 0,
			"You can not pass bytecode size as zero bytes");

		// Setting binding function for constant buffers
		std::function<void(UINT)> functionCB = std::bind(
			&RenderingDeviceContext::BatchBindConstantBufferDS,
			pRenderingDeviceContext.get(), std::placeholders::_1);

		m_ConstantBufferBatch.SetBindingFunction(functionCB);

		// Setting binding function for sampler states
		std::function<void(UINT)> functionSS = std::bind(
			&RenderingDeviceContext::BatchBindSamplerStateDS,
			pRenderingDeviceContext.get(), std::placeholders::_1);

		m_SamplerStateBatch.SetBindingFunction(functionSS);

		// Create domain shader from specific API(e.g DirectX, Opengl etc.)
		return CreateApiSpecific(pShaderBytecode, byteCodeSize,
			pRenderingDevice);
	}

	// Binding shader, constant buffers and sampler states into domain stage
	// If shader is allready binded then binding is skipped
	// Buffers and samplers which are allready binded are not binded again
	// Buffers and samplers are binded in groups(batch binding)
	// Parameters:
	// pRenderingDeviceContext -> smart pointer to rendering device context, can not be null
	// This function is virtual
	void VBind(const std::shared_ptr<RenderingDeviceContext> &
		pRenderingDeviceContext) override
	{
		// First check if this shader is not allready binded from last frame
		// If it is allready binded than do nothing
		if (pRenderingDeviceContext->GetRenderingPipelineTable().TryBindShader(
			GetShaderAddress(), static_cast<UINT>(ShaderType::DOMAIN_SHADER)))
		{
			// If not bind shader to specific slot
			BindApiSpecifc(pRenderingDeviceContext);

			// and bind constant buffers and sampler states too
			// Allready binded constant buffers and sampler states are skipped
			// and binding is performed in batch
			m_ConstantBufferBatch.Bind(pRenderingDeviceContext);
			m_SamplerStateBatch.Bind(pRenderingDeviceContext);
		}
	}
};