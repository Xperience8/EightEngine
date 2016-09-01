#pragma once

#include "EightEngine\Graphics\ApiWrapper\GraphicsFlags.h"

class RenderingDevice;
class RenderingDeviceContext;

class D3D11ConstantBuffer
{
protected:
	D3D11ConstantBuffer() : m_pConstantBuffer(nullptr) { }
	~D3D11ConstantBuffer() 
	{ 
		if (m_pConstantBuffer)
		{
			m_pConstantBuffer->Release();
		}
	}

	bool CreateApiSpecific(UINT bufferSize,
		const std::shared_ptr<RenderingDevice> &pRenderingDevice);
	bool UpdateBufferApiSpecific(const void *pNewData, UINT bufferSize,
		const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext);
	void BatchBindApiSpecific(const std::shared_ptr<RenderingDeviceContext> &
		pRenderingDeviceContext);	
private:
	ID3D11Buffer *m_pConstantBuffer;
};