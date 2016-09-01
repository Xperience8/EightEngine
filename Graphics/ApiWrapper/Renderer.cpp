#include "stdafx.h"
#include "Renderer.h"
#include "EightEngine\GameAppLayer\Screen.h"
#include "EightEngine\Graphics\EffectCache.h"
#include "EightEngine\Graphics\ConstantBufferCache.h"
#include "EightEngine\Graphics\IRenderableEntity.h"
#include "EightEngine\Graphics\Rendering\RenderingScene.h"
#include "EightEngine\EventManager\EventManager.h"
#include "EightEngine\EventManager\Events\DebugRenderingEventData.h"
#include "EightEngine\ResourceCache\ResourceCache.h"

Renderer g_Renderer;

bool Renderer::StartUp(const XmlNode &configNode, const std::shared_ptr<Screen> &pScreen)
{
	if (!CreateDeviceApiSpecific())
	{
		return false;
	}

	//
	m_pRenderingStatesCache = std::make_shared<RenderingStatesCache>();
	if (!m_pRenderingStatesCache->Init(m_pRenderingDevice))
	{
		return false;
	}
	//


	// Create loaders which are needed for loading graphics stuff
	auto shadersNode = configNode.GetChildrenNode("Shaders");
	std::unique_ptr<IResourceLoader> pShaderLoader = std::make_unique<ShaderResourceLoader>(
		m_pRenderingStatesCache, shadersNode.GetAttribute("shaderConfigXML").GetValue());
	std::unique_ptr<IResourceLoader> pFontLoader = std::make_unique<FontLoader>();
	std::unique_ptr<IResourceLoader> pTextureLoader = std::make_unique<TextureResourceLoader>();
	std::unique_ptr<IResourceLoader> pModelLoader = std::make_unique<RenderingModelResourceLoader>();

	g_ResourceCache.AddLoader(pShaderLoader, ShaderResourceLoader::s_SIGNATURE);
	g_ResourceCache.AddLoader(pFontLoader, FontLoader::s_SIGNATURE);
	g_ResourceCache.AddLoader(pTextureLoader, TextureResourceLoader::s_SIGNATURE);	
	g_ResourceCache.AddLoader(pModelLoader, RenderingModelResourceLoader::s_SIGNATURE);

	m_pEffectCache = std::make_shared<EffectCache>();
	if (!m_pEffectCache->Init(shadersNode, m_pRenderingStatesCache))
	{
		return false;
	}

	m_pRenderingScene = std::make_shared<RenderingScene>();
	m_pRenderingScene->Initialize(*this);

	m_pScreen = pScreen;
	if (!m_BackBuffer.Init(configNode, m_pScreen->GetScreenHandle(), m_pRenderingDevice))
	{
		return false;
	}
	m_pRenderTarget = &(m_BackBuffer.GetRenderTarget());

	g_EventManager.AddListener(EventListener(this, &Renderer::ActivateDebugRendering),
		DebugRenderingEventData::s_TYPE);

	m_pRenderingScene->RegisterEvents();
	m_pScreen->RegisterEvents();

	return true;
}
void Renderer::ShutDown()
{
	m_pScreen->UnregisterEvents();
	m_pRenderingScene->UnregisterEvents();

	g_EventManager.RemoveListener(reinterpret_cast<UINT>(this),
		DebugRenderingEventData::s_TYPE);

	m_pRenderingScene.reset();

	g_ResourceCache.RemoveLoader(TextureResourceLoader::s_SIGNATURE);
	g_ResourceCache.RemoveLoader(FontLoader::s_SIGNATURE);
	g_ResourceCache.RemoveLoader(ShaderResourceLoader::s_SIGNATURE);

	m_pEffectCache.reset();
	m_pRenderingStatesCache.reset();

	m_pRenderTarget = nullptr;
	m_BackBuffer.Destroy();
	m_pScreen.reset();

	m_pRenderingDeviceContext.reset();
	m_pRenderingDevice.reset();
}
bool Renderer::OnResize(UINT resX, UINT resY)
{
	m_pRenderingDeviceContext->GetContext3D()->ClearState();

	m_pScreen->Resize(resX, resY);

	return m_BackBuffer.Resize(resX, resY, m_pRenderingDevice);
}
void Renderer::ActivateDebugRendering(const EventPtr<IEventData> &pEvent)
{
	auto pEventData = StaticPointerCast<DebugRenderingEventData>(pEvent);

	m_DebugRenderingStatus = pEventData->GetStatus();
}
void Renderer::Render()
{
	for (auto &pCamera : m_CameraList)
	{
		m_pRenderingScene->Update(pCamera, *this);
		m_pRenderingScene->Render(pCamera, *this);
	}

	m_CameraList.clear();
}

UINT Renderer::Picking(UINT scX, UINT scY, const std::shared_ptr<GameCamera> &pCamera)
{
	DirectX::XMMATRIX proj = pCamera->GetProjMatrix();

	float vx = (((2.f * scX) / m_pScreen->GetScreenWidth()) - 1.f) / proj.r[0].m128_f32[0];
	float vy = (((-2.f * scY) / m_pScreen->GetScreenHeight()) + 1.f) / proj.r[1].m128_f32[1];

	DirectX::XMVECTOR rayOrigin = DirectX::XMVectorZero();
	DirectX::XMVECTOR rayDirection = DirectX::XMVectorSet(vx, vy, 1.f, 0.f);

	DirectX::XMMATRIX invView = DirectX::XMMatrixInverse(&DirectX::XMMatrixDeterminant(
		pCamera->GetViewMatrix()), pCamera->GetViewMatrix());

	rayOrigin = DirectX::XMVector3TransformCoord(rayOrigin, invView);
	rayDirection = DirectX::XMVector3Normalize(DirectX::XMVector3TransformNormal(rayDirection, invView));

	return m_pRenderingScene->Picking(rayOrigin, rayDirection);
}




//
RenderingPipelineTable::RenderingPipelineTable() :
m_ShadersMap(static_cast<UINT>(ShaderType::ShadersCount)),
m_ConstantBuffersMap(static_cast<UINT>(ShaderType::ShadersCount) * 15),
m_SamplerStatesMap(static_cast<UINT>(ShaderType::ShadersCount) * 15),
m_ShaderResourceViewsMap(static_cast<UINT>(ShaderType::ShadersCount) * 15),
m_RasterizerStateId(0), m_BlendStateId(0), m_DepthStencilStateId(0),
m_InputLayoutId(0), m_PrimitiveTopologyId(0), m_VertexBufferId(0),
m_IndexBufferId(0)
{
	memset(m_ShadersMap.data(), EMPTY_SLOT, m_ShadersMap.size());
	memset(m_ShaderResourceViewsMap.data(), EMPTY_SLOT, m_ShaderResourceViewsMap.size());
	memset(m_ConstantBuffersMap.data(), EMPTY_SLOT, m_ConstantBuffersMap.size());
	memset(m_SamplerStatesMap.data(), EMPTY_SLOT, m_SamplerStatesMap.size());
}
//