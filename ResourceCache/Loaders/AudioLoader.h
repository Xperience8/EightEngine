#pragma once

#include "IResourceLoader.h"

class AudioManager;

class AudioLoader : public IResourceLoader
{
public:
	static const UINT s_SIGNATURE = 0xDEADC0E0;

	AudioLoader() { }
	~AudioLoader() { }

	// Only wave format
	bool VCreateResource(BYTE *pRawData, UINT rawDataSize, const std::string &resourceName,
		std::unique_ptr<ResourceHandleBase> &pResourceHandle) override;
};