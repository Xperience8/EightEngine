#pragma once

#include "IResourceLoader.h"

class Mesh;
class Material;
class MemoryReader;

class RenderingModelResourceLoader : public IResourceLoader
{
private:
	bool LoadMesh(Mesh &renderingMesh, MemoryReader &memoryReader);
	void LoadMaterial(Material &renderingMaterial, MemoryReader &memoryReader);
public:
	static const UINT s_SIGNATURE = 0xDEADC0DE;

	RenderingModelResourceLoader() { }
	~RenderingModelResourceLoader() { }

	bool VCreateResource(BYTE *pRawMemory, UINT MemorySize, const std::string &name,
		std::unique_ptr<ResourceHandleBase> &pResource) override;
};