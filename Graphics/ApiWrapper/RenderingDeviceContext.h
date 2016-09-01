#pragma once

#include "EightEngine\Graphics\DirectX11\D3D11RenderingDeviceContext.h"

//
#include "EightEngine\Graphics\ConstantBufferCache.h"
//

class RenderingDevice;

class RenderingDeviceContext : public D3D11RenderingDeviceContext
{
public:
	RenderingDeviceContext(Microsoft::WRL::ComPtr<ID3D11DeviceContext> &pContext3D)
		: D3D11RenderingDeviceContext(pContext3D) { }
	~RenderingDeviceContext() { }
	
	//
	bool Init(const std::shared_ptr<RenderingDevice> &pRenderingDevice);
	//


	RenderingPipelineTable &GetRenderingPipelineTable() { return m_RenderingPipeline; }

	std::shared_ptr<ConstantBuffer> &GetConstantBuffer(const std::string &name)
	{
		return m_ConstantBufferCache.GetBuffer(name);
	}
private:
	RenderingPipelineTable m_RenderingPipeline;
	ConstantBufferCache m_ConstantBufferCache;
};


