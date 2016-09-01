# pragma once

#include "EightEngine\Graphics\DirectX11\States\D3D11DepthStencilState.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDeviceContext.h"

class RenderingDevice;

class DepthStencilState : public D3D11DepthStencilState
{
public:
	DepthStencilState() { }
	~DepthStencilState() { }

	// Creating Depth stencil state
	// Parameters:
	// pRenderingDevice -> smart pointer to rendering device, it can not be null
	bool Create(const std::shared_ptr<RenderingDevice> &pRenderingDevice)
	{
		return CreateApiSpecific(pRenderingDevice);
	}
	// Binding depth stencil state into rendering pipeline
	// If this depth stencil state is allready binded then this call is skipped
	void Bind(const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
	{
		if (pRenderingDeviceContext->GetRenderingPipelineTable().TryBindDepthStencilState(
			GetStateAddress()))
		{
			BindApiSpecific(pRenderingDeviceContext);
		}
	}
};