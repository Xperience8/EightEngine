#include "stdafx.h"
#include "D3D11Texture2D.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDevice.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDeviceContext.h"

bool D3D11Texture2D::InitApiSpecific(const SubresourceData *pSubData,
	const std::shared_ptr<RenderingDevice> &pRenderingDevice)
{
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice3D = pRenderingDevice->GetDevice3D();
	D3D11_TEXTURE2D_DESC desc = TranslateEngineTextureDescToDirectX();
	HRESULT hr;
	if (pSubData)
	{
		// Create Texture and fill it with data
		hr = pDevice3D->CreateTexture2D(&desc,
			reinterpret_cast<const D3D11_SUBRESOURCE_DATA*>(pSubData), &m_pTexture);
	}
	else {
		hr = pDevice3D->CreateTexture2D(&desc, nullptr, &m_pTexture);
	}
	if (FAILED(hr))
	{
		EIGHT_ERROR("DirectX Error: Can not create texture from memory");
		return false;
	}
	// Create Shader Resource View
	if (m_BindFlag & BindFlag::BIND_SHADER_RESOURCE)
	{
		hr = pDevice3D->CreateShaderResourceView(m_pTexture, nullptr, &m_pTextureSRV);

		if (FAILED(hr))
		{
			EIGHT_ERROR("DirectX Error: Can not create shader resource view from texture");
			return false;
		}
	}
	// Create Render Target View
	if (m_BindFlag & BindFlag::BIND_RENDER_TARGET)
	{
		hr = pDevice3D->CreateRenderTargetView(m_pTexture, nullptr, &m_pTextureRTV);
		
		if (FAILED(hr))
		{
			EIGHT_ERROR("DirectX Error: Can not create render target view to texture");
			return false;
		}
	}

	return true;
}
void D3D11Texture2D::BatchBindApiSpecific(
	const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
{
	//pRenderingDeivceContext->BindPixelShaderResource(m_pTextureSRV);
	pRenderingDeviceContext->BindShaderResourceViewInBatch(m_pTextureSRV);
}
D3D11_TEXTURE2D_DESC D3D11Texture2D::TranslateEngineTextureDescToDirectX()
{
	// Translate engine texture description to directX description
	D3D11_TEXTURE2D_DESC directXTextureDesc;
	directXTextureDesc.ArraySize = m_TextureDesc.ArraySize;
	directXTextureDesc.BindFlags = m_BindFlag;
	directXTextureDesc.CPUAccessFlags = m_CpuAccessFlag;
	directXTextureDesc.Format = static_cast<DXGI_FORMAT>(m_TextureDesc.Format);
	directXTextureDesc.Width = m_TextureDesc.TextureWidth;
	directXTextureDesc.Height = m_TextureDesc.TextureHeight;
	directXTextureDesc.MipLevels = m_TextureDesc.MipLevels;
	directXTextureDesc.Usage = static_cast<D3D11_USAGE>(m_UsageFlag);
	directXTextureDesc.MiscFlags = m_TextureDesc.MiscFlags;
	directXTextureDesc.SampleDesc.Count = 1;
	directXTextureDesc.SampleDesc.Quality = 0;

	return directXTextureDesc;
}