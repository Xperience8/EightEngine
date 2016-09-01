#pragma once

struct SubresourceData
{
	// Texels from one mip map level or buffer data
	const BYTE *pSysMem;
	// Only used in 2D or 3D textures, it says how many bytes skip to the next row 
	UINT SysMemPitch;
	// Only used in 3D textures, it says how many bytes to skip to the next layer
	UINT SysMemSlicePitch;
};