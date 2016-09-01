#pragma once

#include "EightEngine\Graphics\DirectX11\Buffers\D3D11VertexBuffer.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDeviceContext.h"

class VertexBuffer : public D3D11VertexBuffer
{
public:
	VertexBuffer() : m_MaxVertexCount(0), m_VertexCount(0) { }
	~VertexBuffer() { }

	// Creating default vertex buffer
	// default means that it will be used only by gpu(only reading)
	// if you want to change this behavior call first SetProperties()
	// Parameters:
	// pVertices	-> pointer to data, can be null
	// vertexCount	-> must be greater than zero
	// vertexStride -> this is size of one vertex, must be greater than zero
	// pRenderingDevice -> smart pointer to rendering device, can not be null
	bool Create(const BYTE *pVertices, UINT vertexCount, UINT vertexStride,
		const std::shared_ptr<RenderingDevice> &pRenderingDevice);

	// Updating vertex buffer with new vertices
	// new vertices are copied into gpu buffer
	// and new number of vertices are set
	// Vertex buffer must be created with cpu access and in dynamic or staging mode
	// Parameters:
	// pNewVertices -> pointer to new vertices, can not be null
	// vertexCount -> must be greater than zero and less than allocated size
	// pRenderingDeviceContext -> smart pointer to rendering device context, can not be null
	bool UpdateVertices(const BYTE *pNewVertices, UINT vertexCount,
		const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext);

	// Binding vertex buffer into input assembler stage
	// If this vertex buffer is allready binded then this call is skipped
	// Parameters:
	// pRenderingDeviceContext -> smart pointer to rendering device context, can not be null
	void Bind(const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
	{
		if (pRenderingDeviceContext->GetRenderingPipelineTable().TryBindVertexBuffer(
			GetAddress()))
		{
			BindApiSpecific(pRenderingDeviceContext);
		}
	}

	// This is used when you want to skip some vertices from the begining
	// Default -> it is set to zero
	// Parameters:
	// offset -> number of vertices to skip, can not be more than allocated number of vertices in gpu buffer
	void SetOffset(UINT offset) 
	{ 
		EIGHT_ASSERT(offset > m_MaxVertexCount, "You can not skip every vertex!");
		m_Offset = offset;
	}

	UINT GetVertexCount() const { return m_VertexCount; }
private:
	// Maximum number of vertices in gpu buffer
	// With this number of vertices is buffer allocated
	UINT m_MaxVertexCount;
	// This cache count of vertices in buffer
	// This is used in dynamic buffer, where you do not want to work with entire buffer
	// but only with subset
	UINT m_VertexCount;
};