#pragma once

#include "EightEngine\Graphics\DirectX11\D3D11Texture2D.h"

class Texture2D : public D3D11Texture2D
{
private:
	// Disable copy constructor and asignment operator
	Texture2D(const Texture2D &texture) { }
	void operator=(const Texture2D &texture) { }

public:
	Texture2D() { }
	~Texture2D() { }

	// Creating texture from subresource data(mip levels) and description
	// If you want to change default texture behavior call first SetProperties method
	// Parameters:
	// pSubData -> pointer to subresource data(to first mip level), it can not be null
	// textureDesc -> describing texture such as dimension, format... but it doesn't describe behavior
	// pRenderingDevice -> smart pointer to rendering device, it can not be null
	bool Create(const SubresourceData *pSubData, const TextureDesc &textureDesc,
		const std::shared_ptr<RenderingDevice> &pRenderingDevice);
	// Binding texture to the rendering pipeline into pixel stage
	// Bindindg is performed in batch
	void BatchBind(const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
	{
		BatchBindApiSpecific(pRenderingDeviceContext);
	}

	// For debug purposes in visual studio visual debugger or nvidia nsight debugger
	void SetTextureName(const std::string &textureName)
	{
		EIGHT_ASSERT(!textureName.empty(),
			"You can not assign empty string as texture name!");
		SetTextureNameApiSpecific(textureName);
	}
};