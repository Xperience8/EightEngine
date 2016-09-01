#pragma once

#include "EightEngine\Graphics\DirectX11\D3D11RenderingDevice.h"

class RenderingDevice : public D3D11RenderingDevice
{
private:

public:
	RenderingDevice(Microsoft::WRL::ComPtr<ID3D11Device> &pDevice3D)
		: D3D11RenderingDevice(pDevice3D) { }
	~RenderingDevice() { }
};

