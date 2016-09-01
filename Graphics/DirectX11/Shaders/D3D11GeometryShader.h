#pragma once

#include "EightEngine\Graphics\ApiWrapper\Shaders\ShaderBase.h"

class D3D11GeometryShader : public ShaderBase
{
protected:
	D3D11GeometryShader() : ShaderBase(ShaderType::GEOMETRY_SHADER),
		m_pGeometryShader(nullptr) { }
	~D3D11GeometryShader()
	{
		if (m_pGeometryShader)
		{
			m_pGeometryShader->Release();
		}
	}

	bool CreateApiSpecific(const BYTE *pShaderBytecode, UINT bytecodeSize,
		const std::shared_ptr<RenderingDevice> &pRenderingDevice);
	void BindApiSpecifc(const std::shared_ptr<RenderingDeviceContext> &
		pRenderingDeviceContext);
	// It is used for determinig whether this shader is allready binded
	// Return value is address of api specific shader
	UINT GetShaderAddress() const { return reinterpret_cast<UINT>(m_pGeometryShader); }

private:
	ID3D11GeometryShader *m_pGeometryShader;
};