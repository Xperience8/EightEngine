#pragma once

#include "IResourceLoader.h"
#include "EightEngine\ResourceCache\ResourceHandleBase.h"

class RawData
{
private:
	BYTE *m_pRawData;
	UINT m_RawDataSize;
public:
	RawData(BYTE *pRawData = nullptr, UINT rawDataSize = 0) : m_pRawData(pRawData),
		m_RawDataSize(rawDataSize) { }
	~RawData() 
	{ 
		delete[] m_pRawData; 
	}

	BYTE *GetRawData() const { return m_pRawData; }
	UINT GetRawDataSize() const { return m_RawDataSize; }
};

class RawDataResourceLoader : public IResourceLoader
{
public:
	static const UINT s_SIGNATURE = 0xDEADC0DA;
	RawDataResourceLoader() { }
	~RawDataResourceLoader() { }
	
	bool VCreateResource(BYTE *pRawMemory, UINT memorySize, const std::string &resourceName,
		std::unique_ptr<ResourceHandleBase> &pResourceHandle) override;
};