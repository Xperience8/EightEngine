#pragma once

#include "EightEngine\Graphics\ApiWrapper\States\SamplerStateBase.h"

class RenderingDevice;
class RenderingDeviceContext;

class D3D11SamplerState : public SamplerStateBase
{
protected:
	D3D11SamplerState() : m_pSamplerState(nullptr) { }
	~D3D11SamplerState()
	{
		if (m_pSamplerState)
		{
			m_pSamplerState->Release();
		}
	}

	bool CreateApiSpecific(const std::shared_ptr<RenderingDevice> &pRenderingDevice);
	void BatchBindApiSpecific(const std::shared_ptr<RenderingDeviceContext> &
		pRenderingDeviceContext);
private:
	ID3D11SamplerState *m_pSamplerState;
};