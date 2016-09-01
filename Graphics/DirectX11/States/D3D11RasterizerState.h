#pragma once

#include "EightEngine\Graphics\ApiWrapper\States\RasterizerStateBase.h"

class RenderingDevice;
class RenderingDeviceContext;

class D3D11RasterizerState : public RasterizerStateBase
{
protected:
	D3D11RasterizerState() : m_pRasterizerState(nullptr) { }
	~D3D11RasterizerState() 
	{ 
		if (m_pRasterizerState)
		{
			m_pRasterizerState->Release();
		}
	}

	bool CreateApiSpecific(const std::shared_ptr<RenderingDevice> &pRenderingDevice);
	void BindApiSpecific(const std::shared_ptr<RenderingDeviceContext> &
		pRenderingDeviceContext);

	// It is used for determinig whether this rasterizer state is allready binded
	// Return value is address of api specific rasterizer state
	UINT GetStateAddress() const { return reinterpret_cast<UINT>(m_pRasterizerState); }

private:
	ID3D11RasterizerState *m_pRasterizerState;
};