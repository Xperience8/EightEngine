#pragma once

#include "EightEngine\Graphics\ApiWrapper\Shaders\ShaderBase.h"

class D3D11DomainShader : public ShaderBase
{
protected:
	D3D11DomainShader() : ShaderBase(ShaderType::DOMAIN_SHADER),
		m_pDomainShader(nullptr) { }
	~D3D11DomainShader()
	{
		if (m_pDomainShader)
		{
			m_pDomainShader->Release();
		}
	}

	bool CreateApiSpecific(const BYTE *pShaderBytecode, UINT bytecodeSize,
		const std::shared_ptr<RenderingDevice> &pRenderingDevice);
	void BindApiSpecifc(const std::shared_ptr<RenderingDeviceContext> &
		pRenderingDeviceContext);
	// It is used for determinig whether this shader is allready binded
	// Return value is address of api specific shader
	UINT GetShaderAddress() const { return reinterpret_cast<UINT>(m_pDomainShader); }

private:
	ID3D11DomainShader *m_pDomainShader;
};