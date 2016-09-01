#pragma once

class D3D11RenderingDevice
{
private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice3D;
public:
	D3D11RenderingDevice(Microsoft::WRL::ComPtr<ID3D11Device> &pDevice3D)
		: m_pDevice3D(pDevice3D) { }
	~D3D11RenderingDevice() { }

	Microsoft::WRL::ComPtr<ID3D11Device> &GetDevice3D() { return m_pDevice3D; }
};