#include "stdafx.h"
#include "D3D11VertexBuffer.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDevice.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDeviceContext.h"

bool D3D11VertexBuffer::CreateApiSpecific(const BYTE *pVertices, UINT vertexCount, 
	UINT vertexStride, const std::shared_ptr<RenderingDevice> &pRenderingDevice)
{
	Microsoft::WRL::ComPtr<ID3D11Device> &pDevice3D = pRenderingDevice->GetDevice3D();

	// Translate engine description into directX
	CD3D11_BUFFER_DESC bufferDesc(vertexCount * vertexStride, D3D11_BIND_VERTEX_BUFFER,
		static_cast<D3D11_USAGE>(m_UsageFlag), m_CpuAccessFlag);

	HRESULT hr;
	// If you want to create empty vertex buffer and fill it with data later
	if (!pVertices)
	{
		EIGHT_ASSERT(m_UsageFlag != UsageFlag::USAGE_IMMUTABLE,
			"You can not create empty buffer if USAGE_IMMUTABLE flag is set");
		hr = pDevice3D->CreateBuffer(&bufferDesc, nullptr, &m_pVertexBuffer);
	}
	else {
		D3D11_SUBRESOURCE_DATA subresourceData;
		subresourceData.pSysMem = pVertices;
		subresourceData.SysMemPitch = 0;
		subresourceData.SysMemSlicePitch = 0;

		hr = pDevice3D->CreateBuffer(&bufferDesc, &subresourceData, &m_pVertexBuffer);
	}
	if (FAILED(hr))
	{
		EIGHT_ERROR("Creating vertex buffer failed!");
		return false;
	}
	
	m_Stride = vertexStride;

	return true;
}
bool D3D11VertexBuffer::UpdateVerticesApiSpecific(const BYTE *pNewVertices, UINT vertexCount,
	const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
{
	// First map data(this mean allocate new buffer, because discard paramter is used)
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HRESULT hr = pRenderingDeviceContext->GetContext3D()->Map(m_pVertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	if (FAILED(hr))
	{
		return false;
	}
	// Then copy data
	memcpy(mappedData.pData, pNewVertices, vertexCount * m_Stride);

	// And unmap buffer(this mean transfer data back to gpu)
	pRenderingDeviceContext->GetContext3D()->Unmap(m_pVertexBuffer, 0);
	
	return true;
}
void D3D11VertexBuffer::BindApiSpecific(
	const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
{
	// Future work binding multiple buffers at the same time -> batching
	pRenderingDeviceContext->GetContext3D()->IASetVertexBuffers(
		0, 1, &m_pVertexBuffer, &m_Stride, &m_Offset);
}