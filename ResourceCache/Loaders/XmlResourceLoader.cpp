#include "stdafx.h"
#include "XmlResourceLoader.h"
#include "EightEngine\ResourceCache\ResourceHandle.h"

typedef ResourceHandle<XmlDocument> XmlResourceHandle;

bool XmlResourceLoader::VCreateResource(BYTE *pRawMemory, UINT memorySize,
	const std::string &resourceName, std::unique_ptr<ResourceHandleBase> &pResource)
{
	// Copy data into new buffer and set zero at the end of the buffer
	BYTE *pResourceRawMemory = new BYTE[memorySize + 1];
	CopyMemory(pResourceRawMemory, pRawMemory, memorySize);
	pResourceRawMemory[memorySize] = 0;

	// If resource handle doesn't exist create it
	// Resource will exist if player request resource from resource cache, 
	//		but resource won't be initialized
	if (!pResource)
	{
		pResource = std::make_unique<XmlResourceHandle>(resourceName);
	}

	// Initialize XmlDocument
	auto &pXmlDoc = static_cast<XmlResourceHandle*>(pResource.get())->GetResource();

	pXmlDoc->Init(reinterpret_cast<char*>(pResourceRawMemory), memorySize + 1);

	// Set that this resource is initialized(if you try to create the same resource twice, 
	//		resource cache won't allow this)
	pResource->PostInit();

	EIGHT_LOG("XmlDocument: " + resourceName + " is loaded and initialized");

	return true;
}