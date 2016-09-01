#pragma once

#include "EightEngine\ResourceCache\ResourceHandleBase.h"

class IResourceLoader
{
public:
	IResourceLoader() { }
	virtual ~IResourceLoader() { }

	virtual bool VCreateResource(BYTE *pRawMemory, UINT memorySize, const std::string &name,
		std::unique_ptr<ResourceHandleBase> &pResource) = 0;
};