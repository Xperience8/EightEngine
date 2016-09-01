#pragma once

#include "EightEngine\Graphics\ApiWrapper\Texture2DBase.h"
#include "EightEngine\Graphics\SubresourceData.h"

class RenderingDevice;
class RenderingDeviceContext;

class D3D11Texture2D : public Texture2DBase
{
private:
	ID3D11Texture2D *m_pTexture;
	ID3D11ShaderResourceView *m_pTextureSRV;
	ID3D11RenderTargetView *m_pTextureRTV;

	D3D11_TEXTURE2D_DESC TranslateEngineTextureDescToDirectX();
protected:
	D3D11Texture2D() : m_pTexture(nullptr), m_pTextureSRV(nullptr), m_pTextureRTV(nullptr) { }
	~D3D11Texture2D() 
	{ 
		if (m_pTexture)
			m_pTexture->Release();
		if (m_pTextureSRV)
			m_pTextureSRV->Release();
		if (m_pTextureRTV)
			m_pTextureRTV->Release();
	}

	bool InitApiSpecific(const SubresourceData *pSubData,
		const std::shared_ptr<RenderingDevice> &pRenderingDevice);
	void BatchBindApiSpecific(
		const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext);
	
	// For debug purposes in visual studio visual debugger or nvidia nsight debugger
	void SetTextureNameApiSpecific(const std::string &textureName)
	{
		EIGHT_ASSERT(m_pTexture,
			"You are trying to set texture name, but texture is not initialized! Initialize it first");
		m_pTexture->SetPrivateData(WKPDID_D3DDebugObjectName,
			static_cast<UINT>(textureName.size()), textureName.c_str());
		if (m_pTextureSRV)
		{
			auto textureSRVName = textureName + "SRV";
			m_pTextureSRV->SetPrivateData(WKPDID_D3DDebugObjectName,
				static_cast<UINT>(textureSRVName.size()), textureSRVName.c_str());
		}
		if (m_pTextureRTV)
		{
			auto textureRTVName = textureName + "RTV";
			m_pTextureRTV->SetPrivateData(WKPDID_D3DDebugObjectName,
				static_cast<UINT>(textureRTVName.size()), textureRTVName.c_str());
		}
	}
};