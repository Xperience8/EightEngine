#pragma once

#include "EightEngine\Graphics\ApiWrapper\States\DepthStencilStateBase.h"

class RenderingDevice;
class RenderingDeviceContext;

class D3D11DepthStencilState : public DepthStencilStateBase
{
protected:
	D3D11DepthStencilState() : m_pDepthStencilState(nullptr) { }
	~D3D11DepthStencilState() 
	{ 
		if (m_pDepthStencilState)
		{
			m_pDepthStencilState->Release();
		}
	}

	bool CreateApiSpecific(const std::shared_ptr<RenderingDevice> &pRenderingDevice);
	void BindApiSpecific(const std::shared_ptr<RenderingDeviceContext> &
		pRenderingDeviceContext);
	// It is used for determinig whether this depth stencil state is allready binded
	// Return value is address of api specific depth stencil state
	UINT GetStateAddress() const { return reinterpret_cast<UINT>(m_pDepthStencilState); }
private:
	ID3D11DepthStencilState *m_pDepthStencilState;
};