#include "stdafx.h"
#include "EightEngine\Graphics\ApiWrapper\Renderer.h"
#include "EightEngine\GameAppLayer\Screen.h"
#include "EightEngine\Graphics\EffectCache.h"
#include "EightEngine\Graphics\ConstantBufferCache.h"
#include "EightEngine\Graphics\IRenderableEntity.h"
#include "EightEngine\Graphics\Rendering\RenderingScene.h"
#include "EightEngine\EventManager\EventManager.h"
#include "EightEngine\EventManager\Events\DebugRenderingEventData.h"

bool D3D11Renderer::CreateDeviceApiSpecific()
{
	UINT CreateDeviceFlag = 0;
#if defined(_DEBUG)
	//CreateDeviceFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice3D;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext3D;
	ThrowIfFailed(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, CreateDeviceFlag,
		nullptr, 0, D3D11_SDK_VERSION, &pDevice3D, &featureLevel, &pContext3D));

	if (featureLevel < D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0)
	{
		return false;
	}

	m_pRenderingDevice = std::make_shared<RenderingDevice>(pDevice3D);
	m_pRenderingDeviceContext = std::make_shared<RenderingDeviceContext>(pContext3D);

	return m_pRenderingDeviceContext->Init(m_pRenderingDevice);
}