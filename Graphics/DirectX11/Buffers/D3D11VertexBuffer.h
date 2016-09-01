#pragma once

#include "EightEngine\Graphics\ApiWrapper\Buffers\BufferBase.h"

class RenderingDevice;
class RenderingDeviceContext;

class D3D11VertexBuffer : public BufferBase
{
protected:
	UINT m_Stride;
	UINT m_Offset;

	D3D11VertexBuffer() : m_Stride(0), m_Offset(0), m_pVertexBuffer(nullptr) { }
	~D3D11VertexBuffer()
	{
		if (m_pVertexBuffer)
		{
			m_pVertexBuffer->Release();
		}
	}

	bool CreateApiSpecific(const BYTE *pVertices, UINT vertexCount, UINT vertexStride,
		const std::shared_ptr<RenderingDevice> &pRenderingDevice);
	bool UpdateVerticesApiSpecific(const BYTE *pNewVertices, UINT vertexCount,
		const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext);
	void BindApiSpecific(const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext);
	
	UINT GetAddress() const { return reinterpret_cast<UINT>(m_pVertexBuffer); }

private:
	ID3D11Buffer *m_pVertexBuffer;
};