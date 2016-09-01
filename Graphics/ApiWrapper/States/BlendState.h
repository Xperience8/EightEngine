#pragma once

#include "EightEngine\Graphics\DirectX11\States\D3D11BlendState.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDeviceContext.h"

class BlendState : public D3D11BlendState
{
public:
	BlendState() { }
	~BlendState() { }

	// Creating blend state
	// Parameters:
	// pRenderingDevice -> smart pointer to rendering device, it can not be null
	bool Create(const std::shared_ptr<RenderingDevice> &pRenderingDevice)
	{
		return CreateApiSpecific(pRenderingDevice);
	}
	// Binding blend state into rendering pipeline
	// If this blend state is allready binded then this call is skipped
	void Bind(const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
	{
		if (pRenderingDeviceContext->GetRenderingPipelineTable().TryBindBlendState(
			GetStateAddress()))
		{
			BindApiSpecific(pRenderingDeviceContext);
		}
	}
};