#include "stdafx.h"
#include "VertexBuffer.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDevice.h"

bool VertexBuffer::Create(const BYTE *pVertices, UINT vertexCount, UINT vertexStride,
	const std::shared_ptr<RenderingDevice> &pRenderingDevice)
{
	EIGHT_ASSERT(pRenderingDevice.get(), "Rendering device isn't created");
	if (vertexCount == 0 || vertexStride == 0)
	{
#if defined(_DEBUG)
		if (vertexCount == 0)
			EIGHT_ERROR("You are trying to create vertex buffer with zero vertices. This is not bad, but you have to set maximum buffer size(vertexCount) and pass empty pointer as vertices");
		if (vertexStride == 0)
			EIGHT_ERROR("Vertex stride can not be zero!");
#endif
		return false;
	}

	m_MaxVertexCount = vertexCount;
	if (pVertices)
	{
		m_VertexCount = vertexCount;
	}

	return CreateApiSpecific(pVertices, vertexCount, vertexStride, pRenderingDevice);
}
bool VertexBuffer::UpdateVertices(const BYTE *pNewVertices, UINT vertexCount,
	const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
{
	EIGHT_ASSERT(m_CpuAccessFlag != CpuAccessFlag::NO_CPU_ACCESS,
		"You can not update vertex buffer without cpu access");
	EIGHT_ASSERT(pNewVertices,
		"pNewVertices can not be null");
	EIGHT_ASSERT(vertexCount,
		"You can not update zero vertices!");
	EIGHT_ASSERT(vertexCount < m_MaxVertexCount,
		"You can not update more vertices than you have allocate in gpu");

	m_VertexCount = vertexCount;

	return UpdateVerticesApiSpecific(pNewVertices, vertexCount, pRenderingDeviceContext);
}