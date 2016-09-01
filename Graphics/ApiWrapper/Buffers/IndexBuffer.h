#pragma once

#include "EightEngine\Graphics\DirectX11\Buffers\D3D11IndexBuffer.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDeviceContext.h"

class IndexBuffer : public D3D11IndexBuffer
{
public:
	IndexBuffer() : m_MaxIndexCount(0), m_IndexCount(0) { }
	~IndexBuffer() { }

	// Creating default index buffer
	// default means that it will be used only by gpu(only reading)
	// if you want to change this behavior call first SetProperties()
	// Parameters:
	// pIndices		-> pointer to data, can be null
	// indexCount	-> maximum number of indices in this buffer, must be greater than zero
	// indexStride -> this is size of one index(2bytes or 4bytes)
	// pRenderingDevice -> smart pointer to rendering device, can not be null
	bool Create(const BYTE *pIndices, UINT indexCount, UINT indexStride,
		const std::shared_ptr<RenderingDevice> &pRenderingDevice);

	// Updating index buffer with new indices
	// New indices are copied into gpu buffer
	// and new number of indices are set
	// Index buffer must be created with cpu access and in dynamic or staging mode
	// Parameters:
	// pNewIndices -> pointer to new indices, can not be null
	// indexCount -> how many indices you want to copied, must be greater than zero and less than allocated size
	// pRenderingDeviceContext -> smart pointer to rendering device context, can not be null
	bool UpdateIndices(const BYTE *pNewIndices, UINT indexCount,
		const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext);

	// Binding index buffer into input assembler stage
	// If this index buffer is allready binded then this call is skipped
	// Parameters:
	// pRenderingDeviceContext -> smart pointer to rendering device context, can not be null
	void Bind(const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
	{
		if (pRenderingDeviceContext->GetRenderingPipelineTable().TryBindIndexBuffer(
			GetAddress()))
		{
			BindApiSpecific(pRenderingDeviceContext);
		}
	}


	// This is used when you want to skip some indices from the begining
	// Default -> it is set to zero(no index is skipped)
	// Parameters:
	// offset -> number of indices to skip, can not be more than allocated number indices in gpu buffer
	void SetOffset(UINT offset)
	{
		EIGHT_ASSERT(offset > m_MaxIndexCount, "You can not skip every index!");
		m_Offset = offset;
	}

	UINT GetIndexCount() const { return m_IndexCount; }
private:
	// Maximum number of indices in gpu buffer
	// With this number of indices is buffer allocated
	UINT m_MaxIndexCount;
	// This cache count of indices in buffer
	// This is used in dynamic buffer, where you do not want to work with entire buffer
	// but only with subset
	UINT m_IndexCount;
};