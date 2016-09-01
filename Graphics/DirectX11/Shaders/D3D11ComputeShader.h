#pragma once

#include "EightEngine\Graphics\ApiWrapper\Shaders\ShaderBase.h"

class D3D11ComputeShader : public ShaderBase
{
protected:
	D3D11ComputeShader() : ShaderBase(ShaderType::COMPUTE_SHADER),
		m_pComputeShader(nullptr) { }
	~D3D11ComputeShader()
	{
		if (m_pComputeShader)
		{
			m_pComputeShader->Release();
		}
	}

	bool CreateApiSpecific(const BYTE *pShaderBytecode, UINT bytecodeSize,
		const std::shared_ptr<RenderingDevice> &pRenderingDevice);
	void BindApiSpecifc(const std::shared_ptr<RenderingDeviceContext> &
		pRenderingDeviceContext);
	// It is used for determinig whether this shader is allready binded
	// Return value is address of api specific shader
	UINT GetShaderAddress() const { return reinterpret_cast<UINT>(m_pComputeShader); }

private:
	ID3D11ComputeShader *m_pComputeShader;
};