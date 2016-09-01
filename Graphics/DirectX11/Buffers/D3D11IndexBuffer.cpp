#include "stdafx.h"
#include "D3D11IndexBuffer.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDevice.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDeviceContext.h"

bool D3D11IndexBuffer::CreateApiSpecific(const BYTE *pIndices, UINT indexCount,
	UINT indexStride, const std::shared_ptr<RenderingDevice> &pRenderingDevice)
{
	Microsoft::WRL::ComPtr<ID3D11Device> &pDevice3D = pRenderingDevice->GetDevice3D();

	// Translate engine description to directX
	CD3D11_BUFFER_DESC bufferDesc(indexCount * indexStride, D3D11_BIND_INDEX_BUFFER,
		static_cast<D3D11_USAGE>(m_UsageFlag), m_CpuAccessFlag);

	HRESULT hr;
	// If you want to create empty index buffer and fill it with data later
	if (!pIndices)
	{
		hr = pDevice3D->CreateBuffer(&bufferDesc, nullptr, &m_pIndexBuffer);
	}
	else {
		D3D11_SUBRESOURCE_DATA subresourceData;
		subresourceData.pSysMem = pIndices;
		subresourceData.SysMemPitch = 0;
		subresourceData.SysMemSlicePitch = 0;

		hr = pDevice3D->CreateBuffer(&bufferDesc, &subresourceData, &m_pIndexBuffer);
	}
	if (FAILED(hr))
	{
		EIGHT_ERROR("Creating index buffer failed!");
		return false;
	}

	m_Format = indexStride == sizeof(UINT) ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;
	m_Stride = indexStride;

	return true;
}
bool D3D11IndexBuffer::UpdateIndicesApiSpecific(const BYTE *pNewIndices, UINT indexCount,
	const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
{
	// First map data(this mean that allocate new buffer, because discard parameter is used)
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HRESULT hr = pRenderingDeviceContext->GetContext3D()->Map(m_pIndexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &mappedData);

	if (FAILED(hr))
	{
		return false;
	}

	// Then copy data
	memcpy(mappedData.pData, pNewIndices, indexCount * m_Stride);

	// And unmap buffer(this mean transfer data back to gpu)
	pRenderingDeviceContext->GetContext3D()->Unmap(m_pIndexBuffer, 0);

	return true;
}
void D3D11IndexBuffer::BindApiSpecific(
	const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
{
	// Future work binding multiple buffers at the same time -> batching
	pRenderingDeviceContext->GetContext3D()->IASetIndexBuffer(
		m_pIndexBuffer, m_Format, m_Offset);
}