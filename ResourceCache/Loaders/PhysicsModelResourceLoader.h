#pragma once

#include "IResourceLoader.h"

class MemoryReader;
class PhysXGamePhysics;
class PhysicsShape;

class PhysicsModelResourceLoader : public IResourceLoader
{
public:
	static const UINT s_SIGNATURE = 0xDEADC0E2;

	PhysicsModelResourceLoader() { }
	~PhysicsModelResourceLoader() { }

	bool VCreateResource(BYTE *pRawMemory, UINT MemorySize, const std::string &name,
		std::unique_ptr<ResourceHandleBase> &pResource) override;
private:
	bool LoadPhysicsShape(MemoryReader &memoryReader, PhysicsShape &physicalShape);
};