#pragma once

#include "GraphicsFlags.h"

struct TextureDesc
{
	FormatFlag Format;

	UINT TextureWidth;
	UINT TextureHeight;

	UINT MipLevels;
	UINT ArraySize;
	UINT MiscFlags;

	TextureDesc(): TextureWidth(0), TextureHeight(0), MipLevels(0), ArraySize(0),
		MiscFlags(0), Format(FormatFlag::FORMAT_UNKNOWN)
	{

	}
	TextureDesc(UINT width, UINT height, FormatFlag format,
		UINT mipLevels = 1, UINT arraySize = 1, UINT miscFlags = 0)
		: TextureWidth(width), TextureHeight(height), Format(format),
		MipLevels(mipLevels), ArraySize(arraySize), MiscFlags(miscFlags)
	{

	}
};
class Texture2DBase
{
protected:
	TextureDesc m_TextureDesc;

	BindFlag m_BindFlag;
	UsageFlag m_UsageFlag;
	CpuAccessFlag m_CpuAccessFlag;

	Texture2DBase() : m_BindFlag(BindFlag::BIND_SHADER_RESOURCE), 
		m_UsageFlag(UsageFlag::USAGE_IMMUTABLE), m_CpuAccessFlag(CpuAccessFlag::NO_CPU_ACCESS) 
	{ 
	
	}
	~Texture2DBase() { }
public:

	// Setting texture properties
	// Default settings are: binding as shader resource, usage is immutable and cpu has no access
	// bindFlag -> how should be texture binded to the rendering pipeline
	// usageFlag -> how should be texture used
	// cpuAccessFlag -> can cpu access texture?
	void SetProperties(BindFlag bindFlag, UsageFlag usageFlag, CpuAccessFlag cpuAccessFlag)
	{
		m_BindFlag = bindFlag;
		m_UsageFlag = usageFlag;
		m_CpuAccessFlag = cpuAccessFlag;
	}

	// Getting texture description:
	// Texture dimension -> width, height
	// Texture format
	// mip levels count, array size and misc flags
	const TextureDesc &GetTextureDesc() const { return m_TextureDesc; }
};