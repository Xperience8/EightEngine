#pragma once

#include "IResourceLoader.h"
#include "EightEngine\Graphics\ApiWrapper\Texture2D.h"

class RenderingDevice;

class TextureResourceLoader : public IResourceLoader
{
public:
	// Used for selecting appropriate loader
	static const UINT s_SIGNATURE = 0xDEADC0DC;
	
	TextureResourceLoader() { }
	~TextureResourceLoader() { }

	bool VCreateResource(BYTE *pRawMemory, UINT memorySize, const std::string &resourceName,
		std::unique_ptr<ResourceHandleBase> &pResourceHandle) override;
};

