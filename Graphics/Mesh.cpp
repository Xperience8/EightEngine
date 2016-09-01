#include "stdafx.h"
#include "Mesh.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDevice.h"

bool Mesh::Init(const MeshDesc &meshDesc, const std::shared_ptr<RenderingDevice> &pRenderingDevice)
{
	m_VertexBuffer.SetProperties(UsageFlag::USAGE_IMMUTABLE, CpuAccessFlag::NO_CPU_ACCESS);
	if (!m_VertexBuffer.Create(reinterpret_cast<BYTE*>(meshDesc.pVerticesMemory),
		meshDesc.VertexCount, meshDesc.VertexBufferStride, pRenderingDevice))
	{
		return false;
	}

	m_IndexBuffer.SetProperties(UsageFlag::USAGE_IMMUTABLE, CpuAccessFlag::NO_CPU_ACCESS);
	if (!m_IndexBuffer.Create(reinterpret_cast<const BYTE*>(meshDesc.pIndicesMemory),
		meshDesc.IndexCount, meshDesc.IndexBufferStride, pRenderingDevice))
	{
		return false;
	}

	return true;
}