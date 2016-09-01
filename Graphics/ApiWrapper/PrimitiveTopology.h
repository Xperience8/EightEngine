#pragma once

#include "EightEngine\Graphics\DirectX11\D3D11PrimitiveTopology.h"
#include "RenderingDeviceContext.h"

class PrimitiveTopology : public D3D11PrimitiveTopology
{
public:
	PrimitiveTopology(PrimitiveTopologyType primitiveTopologyType)
		: D3D11PrimitiveTopology(primitiveTopologyType) { }
	~PrimitiveTopology() { }

	// Binding primitive topology into input assembler stage
	// If this primitive topology is allready binded then this call is skipped
	void Bind(const std::shared_ptr<RenderingDeviceContext> &
		pRenderingDeviceContext)
	{
		if (pRenderingDeviceContext->GetRenderingPipelineTable().TryBindPrimitiveTopology(
			GetAddress()))
		{
			BindApiSpecific(pRenderingDeviceContext);
		}
	}
};