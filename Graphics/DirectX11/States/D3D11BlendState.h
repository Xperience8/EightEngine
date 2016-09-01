#pragma once

#include "EightEngine\Graphics\ApiWrapper\States\BlendStateBase.h"

class RenderingDevice;
class RenderingDeviceContext;

class D3D11BlendState : public BlendStateBase
{
protected:
	D3D11BlendState() : m_pBlendState(nullptr) { }
	~D3D11BlendState()
	{
		if (m_pBlendState)
		{
			m_pBlendState->Release();
		}
	}

	bool CreateApiSpecific(const std::shared_ptr<RenderingDevice> &pRenderingDevice);
	void BindApiSpecific(const std::shared_ptr<RenderingDeviceContext> &
		pRenderingDeviceContext);
	// It is used for determinig whether this blend state is allready binded
	// Return value is address of api specific blend state
	UINT GetStateAddress() const { return reinterpret_cast<UINT>(m_pBlendState); }
private:
	ID3D11BlendState *m_pBlendState;
};