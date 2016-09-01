#include "stdafx.h"
#include "TextureResourceLoader.h"
#include "EightEngine\ResourceCache\ResourceHandle.h"
#include "EightEngine\Graphics\ApiWrapper/RenderingDevice.h"

#include "EightEngine\Graphics\DDSTextureLoader.h"
#include "EightEngine\Graphics\ApiWrapper\Renderer.h"

typedef ResourceHandle<Texture2D> TextureResourceHandle;

bool TextureResourceLoader::VCreateResource(BYTE *pRawMemory, UINT memorySize,
	const std::string &resourceName, std::unique_ptr<ResourceHandleBase> &pResourceHandle)
{
	// If resource handle was not requested, create it
	if (!pResourceHandle)
	{
		pResourceHandle = std::make_unique<TextureResourceHandle>(resourceName);
	}

	// Get texture from resource handle
	auto &pTexture = static_cast<TextureResourceHandle*>(pResourceHandle.get())->GetResource();

	// Default settings: 
	//		texture will be only for read by GPU
	//		texture is created as shader resource a.k.a it can be used in shaders(for sampling by GPU)
	//		cpu has no access
	pTexture->SetProperties(BindFlag::BIND_SHADER_RESOURCE,
		UsageFlag::USAGE_IMMUTABLE, CpuAccessFlag::NO_CPU_ACCESS);

	TextureDesc textureDesc;
	// Data are loaded from DDS stream
	// They are automatically decompressed
	std::unique_ptr<SubresourceData[]> pInitData;
	if (!LoadDDSTextureData(pRawMemory, memorySize, pInitData, textureDesc))
	{
		EIGHT_WARNING("Can not load texture " + resourceName + " from memory stream");
		return false;
	}
	if (!pTexture->Create(pInitData.get(), textureDesc, g_Renderer.GetRenderingDevice()))
	{
		EIGHT_WARNING("Can not initialize texture " + resourceName);
		return false;
	}

#if defined(_DEBUG)
	// Erase from name path and extension and set it as texture name
	std::string textureDebugName = resourceName.substr(
		resourceName.find_last_of("/") + 1, std::string::npos);
	textureDebugName = textureDebugName.substr(0, textureDebugName.size() - 4);
	pTexture->SetTextureName(textureDebugName);
#endif

	//pResourceHandle->SetInitializationStatus(true);
	pResourceHandle->PostInit();


	EIGHT_LOG("Texture: " + resourceName + " is loaded and initialized");

	return true;
}