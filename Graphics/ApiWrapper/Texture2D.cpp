#include "stdafx.h"
#include "Texture2D.h"
#include "RenderingDevice.h"

bool Texture2D::Create(const SubresourceData *pSubData, const TextureDesc &textureDesc,
	const std::shared_ptr<RenderingDevice> &pRenderingDevice)
{
	m_TextureDesc = textureDesc;

	EIGHT_ASSERT(m_TextureDesc.TextureWidth != 0,
		"You can not create 2D texture with zero width!");
	EIGHT_ASSERT(m_TextureDesc.TextureHeight != 0,
		"You can not create 2D texture with zero height!");
	EIGHT_ASSERT(m_TextureDesc.MipLevels != 0,
		"You can not create texture with zero mip levels. There must be at least one level");
	EIGHT_ASSERT(m_TextureDesc.ArraySize != 0,
		"You can not create 2D texture with zero array size. If you want to create normal 2D texture(not texture array) this must be one");
	EIGHT_ASSERT(m_TextureDesc.Format != FormatFlag::FORMAT_UNKNOWN,
		"You can not create 2D texture with unknown format");

	return InitApiSpecific(pSubData, pRenderingDevice);
}