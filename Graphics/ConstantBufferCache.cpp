#include "stdafx.h"
#include "ConstantBufferCache.h"
#include "EightEngine\Graphics\ApiWrapper\Buffers\ConstantBuffer.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDevice.h"

const char *ModelCBData::NAME = "ModelCB";
const char *CameraCBData::NAME = "CameraCB";
const char *MaterialCBData::NAME = "MaterialCB";
const char *LightCBData::NAME = "LightCB";
const char *TerrainCBData::NAME = "TerrainCB";
const char *TextCBData::NAME = "TextCB";


bool ConstantBufferCache::OnRestore(const std::shared_ptr<RenderingDevice> &pRenderingDevice)
{
	if (m_ConstantBuffersMap.empty())
		m_ConstantBuffersMap.clear();

	std::shared_ptr<ConstantBuffer> pBuffer;

	pBuffer = std::make_shared<ConstantBuffer>();
	if (!pBuffer->Create(sizeof(ModelCBData), pRenderingDevice))
	{
		return false;
	}
	m_ConstantBuffersMap.emplace(ModelCBData::NAME, pBuffer);

	pBuffer = std::make_shared<ConstantBuffer>();
	if (!pBuffer->Create(sizeof(CameraCBData), pRenderingDevice))
	{
		return false;
	}
	m_ConstantBuffersMap.emplace(CameraCBData::NAME, pBuffer);

	pBuffer = std::make_shared<ConstantBuffer>();
	if (!pBuffer->Create(sizeof(LightCBData), pRenderingDevice))
	{
		return false;
	}
	m_ConstantBuffersMap.emplace(LightCBData::NAME, pBuffer);

	pBuffer = std::make_shared<ConstantBuffer>();
	if (!pBuffer->Create(sizeof(MaterialCBData), pRenderingDevice))
	{
		return false;
	}
	m_ConstantBuffersMap.emplace(MaterialCBData::NAME, pBuffer);

	pBuffer = std::make_shared<ConstantBuffer>();
	if (!pBuffer->Create(sizeof(TerrainCBData), pRenderingDevice))
	{
		return false;
	}
	m_ConstantBuffersMap.emplace(TerrainCBData::NAME, pBuffer);

	pBuffer = std::make_shared<ConstantBuffer>();
	if (!pBuffer->Create(sizeof(TextCBData), pRenderingDevice))
	{
		return false;
	}
	m_ConstantBuffersMap.emplace(TextCBData::NAME, pBuffer);

	return true;
}
