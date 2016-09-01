#include "stdafx.h"
#include "IndexBuffer.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDevice.h"

bool IndexBuffer::Create(const BYTE *pIndices, UINT indexCount, UINT indexStride,
	const std::shared_ptr<RenderingDevice> &pRenderingDevice)
{
	EIGHT_ASSERT(pRenderingDevice.get(), "Rendering device isn't created");
	if (indexCount == 0 || indexStride == 0)
	{
#if defined(_DEBUG)
		if (indexCount == 0)
			EIGHT_ERROR("You are trying to create index buffer with zero Indices. This is not invalid, but you have to set maximum buffer size(indexCount) and pass empty pointer to indices");
		if (indexStride == 0)
			EIGHT_ERROR("Index stride can not be zero!");
#endif
		return false;
	}

	m_MaxIndexCount = indexCount;
	if (pIndices)
	{
		m_IndexCount = indexCount;
	}

	return CreateApiSpecific(pIndices, indexCount, indexStride, pRenderingDevice);
}
bool IndexBuffer::UpdateIndices(const BYTE *pNewIndices, UINT indexCount,
	const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
{
	EIGHT_ASSERT(m_CpuAccessFlag != CpuAccessFlag::NO_CPU_ACCESS,
		"You can not update index buffer without cpu access");
	EIGHT_ASSERT(pNewIndices,
		"pNewIndices can not be null");
	EIGHT_ASSERT(indexCount,
		"You can not update zero indices!");
	EIGHT_ASSERT(indexCount < m_MaxIndexCount,
		"You can not update more indices than you have allocate in gpu");

	m_IndexCount = indexCount;

	return UpdateIndicesApiSpecific(pNewIndices, indexCount, pRenderingDeviceContext);
}