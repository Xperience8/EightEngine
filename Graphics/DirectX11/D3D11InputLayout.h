#pragma once

#include "EightEngine\Graphics\ApiWrapper\InputLayoutBase.h"

class RenderingDevice;
class RenderingDeviceContext;

class D3D11InputLayout : public InputLayoutBase
{
public:
	bool IsInitialized() const { return m_pInputLayout != nullptr; }
protected:
	D3D11InputLayout() : m_pInputLayout(nullptr) { }
	~D3D11InputLayout()
	{
		if (m_pInputLayout)
		{
			m_pInputLayout->Release();
		}
	}

	bool CreateApiSpecific(const void *pShaderBytecode, UINT bytecodeSize,
		const std::shared_ptr<RenderingDevice> &pRenderingDevice);
	void BindApiSpecific(const std::shared_ptr<RenderingDeviceContext> &
		pRenderingDeviceContext);

	// It is used for determinig whether this input layout is allready binded
	// Return value is address of api specific input layout
	UINT GetAddress() const { return reinterpret_cast<UINT>(m_pInputLayout); }

private:
	ID3D11InputLayout *m_pInputLayout;

	void TranslateEngineDesc(const InputElementDesc &inputElementDesc,
		D3D11_INPUT_ELEMENT_DESC &inputElementDescDirectX);
};