#pragma once

#include "IResourceLoader.h"
#include "EightEngine\Graphics\UserInterface\FontSheet.h"

class RenderingDevice;

class FontLoader : public IResourceLoader
{
public:
	static const UINT s_SIGNATURE = 0xDEADC0E1;

	FontLoader() { }
	~FontLoader() { }

	bool VCreateResource(BYTE *pRawData, UINT rawDataSize, const std::string &resourceName,
		std::unique_ptr<ResourceHandleBase> &pResourceHandle) override;
};