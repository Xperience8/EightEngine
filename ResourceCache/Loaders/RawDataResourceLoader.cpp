#include "stdafx.h"
#include "RawDataResourceLoader.h"
#include "EightEngine\ResourceCache\ResourceHandle.h"

typedef	ResourceHandle<RawData> RawDataResourceHandle;

bool RawDataResourceLoader::VCreateResource(BYTE *pRawMemory, UINT memorySize,
	const std::string &resourceName, std::unique_ptr<ResourceHandleBase> &pResourceHandle)
{
	// Create new array, copy memory, and create raw data resource
	BYTE *pResourceRawMemory = new BYTE[memorySize];
	CopyMemory(pResourceRawMemory, pRawMemory, memorySize);
	
	if (!pResourceHandle)
	{
		pResourceHandle = std::make_unique<RawDataResourceHandle>(resourceName);
	}

	auto &pResource = static_cast<RawDataResourceHandle*>(pResourceHandle.get())->GetResource();
	pResource = std::make_shared<RawData>(pResourceRawMemory, memorySize);



	pResourceHandle->PostInit();

	EIGHT_LOG("RawData: " + resourceName + " is loaded and initialized");

	return true;
}