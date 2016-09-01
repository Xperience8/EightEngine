#pragma once

#include "EightEngine\Graphics\DirectX11\States\D3D11SamplerState.h"

class SamplerState : public D3D11SamplerState
{
public:
	SamplerState() { }
	~SamplerState() { }
	
	// Create Sampler State with properties set by SetProperties method
	// Parameters:
	// pRenderingDevice -> smart pointer to rendering device, 
	//					   it can not be null(it's mean not initialized)
	bool Create(const std::shared_ptr<RenderingDevice> &pRenderingDevice)
	{
		return CreateApiSpecific(pRenderingDevice);
	}
	// Binding sampler state to rendering pipeline
	// Binding is performed in batch, it's mean that this function call 
	// do not automatically bind sampler state to the slot, but it is binded to batch
	// and after call BatchBindSamplerStateVS/HS/GS... they are set into rendering pipeline
	// Parameters
	// pRenderingDevice -> smart pointer to rendering device context, 
	//					   it can not be null(it's mean not initialized)
	void BatchBind(const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
	{
		BatchBindApiSpecific(pRenderingDeviceContext);
	}
};