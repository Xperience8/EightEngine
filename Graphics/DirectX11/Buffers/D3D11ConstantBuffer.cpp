#include "stdafx.h"
#include "D3D11ConstantBuffer.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDevice.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDeviceContext.h"

bool D3D11ConstantBuffer::CreateApiSpecific(UINT bufferSize, 
	const std::shared_ptr<RenderingDevice> &pRenderingDevice)
{
	CD3D11_BUFFER_DESC bufferDesc(bufferSize, D3D11_BIND_CONSTANT_BUFFER,
		D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);

	Microsoft::WRL::ComPtr<ID3D11Device> &pDevice3D = pRenderingDevice->GetDevice3D();

	HRESULT hr = pDevice3D->CreateBuffer(&bufferDesc, nullptr, &m_pConstantBuffer);
	if (FAILED(hr))
		return false;

	return true;
}
bool D3D11ConstantBuffer::UpdateBufferApiSpecific(const void *pNewData, UINT bufferSize,
	const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
{
	// First map data(this mean allocate new buffer, because discard paramter is used)
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> &pDeviceContext = pRenderingDeviceContext->GetContext3D();

	D3D11_MAPPED_SUBRESOURCE mappedData;
	HRESULT hr = pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 
		0, &mappedData);

	if (FAILED(hr))
	{
		return false;
	}
	// Then copy data
	memcpy(mappedData.pData, pNewData, bufferSize);

	// And unmap buffer(this mean transfer data back to gpu)
	pDeviceContext->Unmap(m_pConstantBuffer, 0);

	return true;
}
void D3D11ConstantBuffer::BatchBindApiSpecific(
	const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
{
	pRenderingDeviceContext->BindConstantBufferInBatch(m_pConstantBuffer);
}