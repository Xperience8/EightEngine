#include "stdafx.h"
#include "D3D11PrimitiveTopology.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDeviceContext.h"

void D3D11PrimitiveTopology::BindApiSpecific(
	const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
{
	pRenderingDeviceContext->GetContext3D()->IASetPrimitiveTopology(m_PrimitiveTopology);
}