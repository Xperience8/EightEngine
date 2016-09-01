#pragma once

#include "EightEngine\Graphics\DirectX11\States\D3D11RasterizerState.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDeviceContext.h"

class RasterizerState : public D3D11RasterizerState
{
public:
	RasterizerState() { }
	~RasterizerState() { }

	// Creating Rasterizer State with properties set by SetProperties method
	// If you want to change default behavior call SetProperties method first
	// Parameters:
	// pRenderingDevice -> smart pointer to rendering device, it can not be null
	bool Create(const std::shared_ptr<RenderingDevice> &pRenderingDevice)
	{
		return CreateApiSpecific(pRenderingDevice);
	}
	// Binding Rasterizer State into rendering pipeline
	// If this state is allready binded then this call is skipped
	void Bind(const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
	{
		if (pRenderingDeviceContext->GetRenderingPipelineTable().TryBindRasterizerState(
			GetStateAddress()))
		{
			BindApiSpecific(pRenderingDeviceContext);
		}
	}
};