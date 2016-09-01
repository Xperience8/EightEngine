#include "stdafx.h"
#include "HeightMapLoader.h"
#include "EightEngine\ResourceCache\ResourceHandle.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDevice.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDeviceContext.h"
#include "EightEngine\Graphics\ApiWrapper\Texture2D.h"
#include "EightEngine\Graphics\DDSTextureLoader.h"

typedef ResourceHandle<HeightMap> HeightMapResourceHandle;

bool HeightMapLoader::VCreateResource(BYTE *pRawMemory, UINT memorySize,
	const std::string &resourceName, std::unique_ptr<ResourceHandleBase> &pResource)
{
	// Only used for getting information like texture width, height, format...
	// This isn't used for creating texture on engine side
	TextureDesc textureDesc;

	// Getting height data from height map 
	std::unique_ptr<SubresourceData[]> pInitData;
	if (!LoadDDSTextureData(pRawMemory, memorySize, pInitData, textureDesc))
	{
		EIGHT_WARNING("Can not load heightmap " + resourceName + " from memory stream");
		return false;
	}

	// We're interested only in top level mip map(if there are more mip maps, they are ignored)
	const BYTE *mappedData = reinterpret_cast<const BYTE*>(pInitData[0].pSysMem);
	UINT texelDataSize = textureDesc.TextureWidth * textureDesc.TextureHeight;
	uint8_t *texelData = new uint8_t[texelDataSize];

	// If there are no padding bytes, copy everything
	if (texelDataSize == pInitData[0].SysMemPitch)
	{
		memcpy(texelData, mappedData, texelDataSize);
	}
	else {
		// Else copy without padding bytes
		// and copy data into this array
		// Padding bytes are discarded
		for (UINT i = 0; i < textureDesc.TextureHeight; i++)
		{
			UINT rowMappedSubresource = i * pInitData[0].SysMemPitch;
			UINT rowNewTexture = i * textureDesc.TextureWidth;

			memcpy(texelData + rowNewTexture, mappedData + rowMappedSubresource,
				textureDesc.TextureWidth);
		}
	}

	// If resource handle was not requested, create it
	if (!pResource)
	{
		pResource = std::make_unique<HeightMapResourceHandle>(resourceName);
	}
	// Initialize height map
	auto &pHeightMap = static_cast<HeightMapResourceHandle*>(
		pResource.get())->GetResource();
	pHeightMap->Init(texelData, textureDesc.TextureWidth, textureDesc.TextureHeight);

	//pResource->SetInitializationStatus(true);
	pResource->PostInit();

	EIGHT_LOG("Heightmap: " + resourceName + " is loaded and initialized");

	return true;
}