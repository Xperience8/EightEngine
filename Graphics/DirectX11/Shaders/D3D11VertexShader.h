#pragma once

#include "EightEngine\Graphics\ApiWrapper\Shaders\ShaderBase.h"

class D3D11VertexShader : public ShaderBase
{
protected:
	D3D11VertexShader() : ShaderBase(ShaderType::VERTEX_SHADER),
		m_pVertexShader(nullptr) { }
	~D3D11VertexShader() 
	{ 
		if (m_pVertexShader)
		{
			m_pVertexShader->Release();
		}
	}

	bool CreateApiSpecific(const BYTE *pShaderBytecode, UINT bytecodeSize,
		const std::shared_ptr<RenderingDevice> &pRenderingDevice);
	void BindApiSpecifc(const std::shared_ptr<RenderingDeviceContext> &
		pRenderingDeviceContext);

	// It is used for determinig whether this shader is allready binded
	// Return value is address of api specific shader
	UINT GetShaderAddress() const { return reinterpret_cast<UINT>(m_pVertexShader); }
private:
	ID3D11VertexShader *m_pVertexShader;
};