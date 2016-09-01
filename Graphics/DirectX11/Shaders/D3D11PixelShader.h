#pragma once

#include "EightEngine\Graphics\ApiWrapper\Shaders\ShaderBase.h"

class D3D11PixelShader : public ShaderBase
{
protected:
	D3D11PixelShader() : ShaderBase(ShaderType::PIXEL_SHADER),
		m_pPixelShader(nullptr) { }
	~D3D11PixelShader()
	{
		if (m_pPixelShader)
		{
			m_pPixelShader->Release();
		}
	}

	bool CreateApiSpecific(const BYTE *pShaderBytecode, UINT bytecodeSize,
		const std::shared_ptr<RenderingDevice> &pRenderingDevice);
	void BindApiSpecifc(const std::shared_ptr<RenderingDeviceContext> &
		pRenderingDeviceContext);
	// It is used for determinig whether this shader is allready binded
	// Return value is address of api specific shader
	UINT GetShaderAddress() const { return reinterpret_cast<UINT>(m_pPixelShader); }

private:
	ID3D11PixelShader *m_pPixelShader;
};