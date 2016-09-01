#pragma once
///////////////////////////////////////////////////////////////////////////////
//					Divide between multiple files							 //
///////////////////////////////////////////////////////////////////////////////
enum class ShaderType 
{ 
	VERTEX_SHADER, 
	HULL_SHADER, 
	DOMAIN_SHADER, 
	GEOMETRY_SHADER, 
	PIXEL_SHADER, 
	COMPUTE_SHADER, 
	ShadersCount 
};

class RenderingDevice;
class RenderingDeviceContext;
class ConstantBuffer;
class SamplerState;

#include "EightEngine\Graphics\BindingBatch.h"

class ShaderBase
{
public:
	// Add constant buffer into batching group
	// buffer added via this function is automatically binded in VBind call
	// and batching binding is performed
	void AddConstantBuffer(const std::shared_ptr<ConstantBuffer> &pConstantBuffer)
	{
		m_ConstantBufferBatch.AddBindingObject(pConstantBuffer);
	}
	// Add sampler state into batching group
	// sampler state added via this function is automatically binded in VBind call
	// and batching binding is performed
	void AddSamplerState(const std::shared_ptr<SamplerState> &pSamplerState)
	{
		m_SamplerStateBatch.AddBindingObject(pSamplerState);
	}

	// Creating shader from memory and creating batching group
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
	virtual bool VCreate(const BYTE *pShaderBytecode, UINT bytecodeSize,
		const std::shared_ptr<RenderingDevice> &pRenderingDevice,
		const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext) = 0;
	
	// Binding shader, constant buffers and sampler states into vertex stage
	// If shader is allready binded then binding is skipped
	// Buffers and samplers which are allready binded are not binded again
	// Buffers and samplers are binded in groups(batch binding)
	// Parameters:
	// pRenderingDeviceContext -> smart pointer to rendering device context, can not be null
	// This function is virtual
	virtual void VBind(const std::shared_ptr<RenderingDeviceContext> &
		pRenderingDeviceContext) = 0;

	// return shader type e.g vertex shader, hull shader ...
	ShaderType GetShaderType() const { return m_ShaderType; }

	virtual ~ShaderBase() { }
protected:
	BindingBatch<ConstantBuffer> m_ConstantBufferBatch;
	BindingBatch<SamplerState> m_SamplerStateBatch;

	ShaderType m_ShaderType;

	ShaderBase(ShaderType shaderType) : m_ShaderType(shaderType),
		m_ConstantBufferBatch(static_cast<UINT>(shaderType) * 15),
		m_SamplerStateBatch(static_cast<UINT>(shaderType) * 15) { }
	
};
///////////////////////////////////////////////////////////////////////////////
//					Divide between multiple files							 //
///////////////////////////////////////////////////////////////////////////////