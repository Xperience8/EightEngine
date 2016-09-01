#pragma once

#include "IResourceLoader.h"
#include "EightEngine\Xml\XmlDocument.h"

class XmlResourceLoader : public IResourceLoader
{
public:
	// Used for selecting appropriate loader
	static const UINT s_SIGNATURE = 0xDEADC0DB;

	XmlResourceLoader() { }
	~XmlResourceLoader() { }

	bool VCreateResource(BYTE *pRawMemory, UINT memorySize, const std::string &resourceName,
		std::unique_ptr<ResourceHandleBase> &pResource) override;
};