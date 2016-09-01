#pragma once

#include "EightEngine\Graphics\ApiWrapper\GraphicsFlags.h"
#include "EightEngine\Graphics\ApiWrapper\Buffers\BufferBase.h"

class RenderingDevice;
class RenderingDeviceContext;

class D3D11IndexBuffer : public BufferBase
{
protected:
	UINT m_Offset;
	UINT m_Stride;

	D3D11IndexBuffer() : m_Offset(0), m_Stride(0), m_Format(DXGI_FORMAT_R16_UINT),
		m_pIndexBuffer(nullptr) { }
	~D3D11IndexBuffer()
	{
		if (m_pIndexBuffer)
		{
			m_pIndexBuffer->Release();
		}
	}

	bool CreateApiSpecific(const BYTE *pIndices, UINT indexCount, UINT indexStride,
		const std::shared_ptr<RenderingDevice> &pRenderingDevice);
	bool UpdateIndicesApiSpecific(const BYTE *pNewIndices, UINT indexCount,
		const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext);
	void BindApiSpecific(const std::shared_ptr<RenderingDeviceContext> &
		pRenderingDeviceContext);

	UINT GetAddress() const { return reinterpret_cast<UINT>(m_pIndexBuffer); }

private:
	ID3D11Buffer *m_pIndexBuffer;
	DXGI_FORMAT m_Format;
};