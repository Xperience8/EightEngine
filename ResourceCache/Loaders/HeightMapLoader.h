#pragma once

#include "IResourceLoader.h"

class HeightMap
{
private:
	const BYTE *m_pHeightMapSamples;
	UINT m_HeightMapWidth;
	UINT m_HeightMapHeight;
public:
	HeightMap() : m_pHeightMapSamples(nullptr), m_HeightMapWidth(0),
		m_HeightMapHeight(0) { }
	HeightMap(BYTE *pHeightMapSamples, UINT heightMapWidth, UINT heightMapHeight)
		: m_pHeightMapSamples(pHeightMapSamples), m_HeightMapWidth(heightMapWidth),
		m_HeightMapHeight(heightMapHeight) { }
	~HeightMap() { delete[] m_pHeightMapSamples; }

	void Init(const BYTE *pHeightMapSamples, UINT heightMapWidth, UINT heightMapHeight)
	{
		m_pHeightMapSamples = pHeightMapSamples;
		m_HeightMapWidth = heightMapWidth;
		m_HeightMapHeight = heightMapHeight;
	}

	UINT GetHeightMapWidth() const { return m_HeightMapWidth; }
	UINT GetHeightMapHeight() const { return m_HeightMapHeight; }
	UINT GetHeightMapSamplesCount() const { return m_HeightMapWidth * m_HeightMapHeight; }
	const BYTE *GetHeightMapSamples() const { return m_pHeightMapSamples; }
};

class HeightMapLoader : public IResourceLoader
{
public:
	// Used for selecting appropriate loader
	static const UINT s_SIGNATURE = 0xDEADC0DF;

	HeightMapLoader() { }
	~HeightMapLoader() { }

	bool VCreateResource(BYTE *pRawData, UINT rawDataSize, const std::string &resourceName,
		std::unique_ptr<ResourceHandleBase> &pResourceHandle) override;
};