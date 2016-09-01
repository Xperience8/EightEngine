#pragma once

#include "EightEngine\Graphics\ApiWrapper\GraphicsFlags.h"

class RenderingDeviceContext;

class D3D11PrimitiveTopology
{
protected:
	D3D11PrimitiveTopology(PrimitiveTopologyType primitiveTopologyType)
		: m_PrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(primitiveTopologyType)) { }
	~D3D11PrimitiveTopology() { }

	void BindApiSpecific(const std::shared_ptr<RenderingDeviceContext> &
		pRenderingDeviceContext);

	UINT GetAddress() const { return static_cast<UINT>(m_PrimitiveTopology); }
private:
	D3D11_PRIMITIVE_TOPOLOGY m_PrimitiveTopology;
};