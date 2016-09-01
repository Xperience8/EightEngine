#include "stdafx.h"
#include "RenderingDeviceContext.h"
#include "RenderingDevice.h"

bool RenderingDeviceContext::Init(const std::shared_ptr<RenderingDevice> &pRenderingDevice)
{
	return m_ConstantBufferCache.OnRestore(pRenderingDevice);
}