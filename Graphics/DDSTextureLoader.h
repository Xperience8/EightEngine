#pragma once

struct TextureDesc;
struct SubresourceData;

bool LoadDDSTextureData(const uint8_t* ddsData, size_t ddsDataSize,
	std::unique_ptr<SubresourceData[]> &pInitData, TextureDesc &textureDesc);

