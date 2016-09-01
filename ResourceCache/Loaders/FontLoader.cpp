#include "stdafx.h"
#include "FontLoader.h"
#include "EightEngine\ResourceCache\ResourceHandle.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDevice.h"
#include "EightEngine\Graphics\ApiWrapper\Texture2D.h"
#include "EightEngine\Memory\MemoryReader.h"
#include "EightEngine\Graphics\ApiWrapper\Renderer.h"

typedef ResourceHandle<FontSheet> FontResourceHandle;

bool FontLoader::VCreateResource(BYTE *pRawData, UINT rawDataSize,
	const std::string &resourceName, std::unique_ptr<ResourceHandleBase> &pResourceHandle)
{
	// Create memory reader
	MemoryReader memoryReader;
	memoryReader.LoadFromMemoryStream(pRawData, rawDataSize);

	// Load basic information
	UINT charHeight = memoryReader.ReadMemory<UINT>();
	UINT spaceWidth = memoryReader.ReadMemory<UINT>();
	CD3D11_RECT *charRectangles = memoryReader.ReadMemoryArray<CD3D11_RECT>(
		FontSheet::NUM_CHARACTERS);

	// Load texture information
	UINT textureWidth = memoryReader.ReadMemory<UINT>();
	UINT textureHeight = memoryReader.ReadMemory<UINT>();
	UINT textureDataSize = memoryReader.ReadMemory<UINT>();
	BYTE *textureData = memoryReader.ReadMemoryArray<BYTE>(textureDataSize);

	UINT stride = textureDataSize / (textureWidth * textureHeight);

	SubresourceData subData;
	subData.pSysMem = textureData;
	subData.SysMemPitch = textureWidth * stride;
	subData.SysMemSlicePitch = 0;

	TextureDesc textureDesc(textureWidth, textureHeight, FormatFlag::FORMAT_R8G8B8A8_UNORM);
	auto pTexture = std::make_shared<Texture2D>();

	if (!pTexture->Create(&subData, textureDesc, g_Renderer.GetRenderingDevice()))
	{
		EIGHT_WARNING("Can not load fontsheet " + resourceName + " from memory stream");
		return false;
	}

	// If resource handle was not requested, create it
	if (!pResourceHandle)
	{
		pResourceHandle = std::make_unique<FontResourceHandle>(resourceName);
	}

	auto &pFontSheet = static_cast<FontResourceHandle*>(pResourceHandle.get())->GetResource();
	pFontSheet->Init(charHeight, spaceWidth, charRectangles, pTexture);

	//pResourceHandle->SetInitializationStatus(true);
	pResourceHandle->PostInit();

	EIGHT_LOG("Fontsheet: " + resourceName + " is loaded and initialized");

	return true;
}