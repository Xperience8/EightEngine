#pragma once

#include "EightEngine\Graphics\DirectX11\Buffers\D3D11ConstantBuffer.h"

class ConstantBuffer : public D3D11ConstantBuffer
{
public:
	ConstantBuffer() : m_BufferSize(0) { }
	~ConstantBuffer() { }

	// Creating constant buffer
	// It will be created as buffer with cpu access for writing and gpu access for reading
	// Parameters:
	// bufferSize -> size of buffer, can not be zero, it is automatically aligned to 16 bytes
	// pRenderingDevice -> smart pointer to rendering device, can not be null
	bool Create(UINT bufferSize, const std::shared_ptr<RenderingDevice> &pRenderingDevice);

	// Updating constant buffer with new data
	// new data are copied into gpu buffer
	// Parameters:
	// pNewData -> pointer to new constant buffer data, can not be null
	// pRenderingDeviceContext -> smart pointer to rendering device context, can not be null
	bool UpdateBuffer(void *pNewData, 
		const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
	{
		EIGHT_ASSERT(pNewData, "You are trying to update data in constant buffer, but pointer to new data is invalid");
		EIGHT_ASSERT(m_BufferSize, "You are trying to update data in constant buffer, but this constant buffer is not initialized");

		return UpdateBufferApiSpecific(pNewData, m_BufferSize, pRenderingDeviceContext);
	}

	// Binding constant buffer into specific shader stage
	// Batch binding means that buffer will not be bound at this function call
	// but later, when RenderingDeviceContext::BatchBatchBindConstantBufferVS/HS/DS... 
	// is called, in this way you can bound multiple buffers in one call to gpu
	void BatchBind(const std::shared_ptr<RenderingDeviceContext> &
		pRenderingDeviceContext)
	{
		BatchBindApiSpecific(pRenderingDeviceContext);
	}
private:
	UINT m_BufferSize;
};