#pragma once

#include "EightEngine\Graphics\ApiWrapper\Shaders\ShaderBase.h"

class D3D11HullShader : public ShaderBase
{
protected:
	D3D11HullShader() : ShaderBase(ShaderType::HULL_SHADER),
		m_pHullShader(nullptr) { }
	~D3D11HullShader() 
	{ 
		if (m_pHullShader)
		{
			m_pHullShader->Release();
		}
	}

	bool CreateApiSpecific(const BYTE *pShaderBytecode, UINT bytecodeSize,
		const std::shared_ptr<RenderingDevice> &pRenderingDevice);
	void BindApiSpecifc(const std::shared_ptr<RenderingDeviceContext> &
		pRenderingDeviceContext);
	// It is used for determinig whether this shader is allready binded
	// Return value is address of api specific shader
	UINT GetShaderAddress() const { return reinterpret_cast<UINT>(m_pHullShader); }

private:
	ID3D11HullShader *m_pHullShader;
};