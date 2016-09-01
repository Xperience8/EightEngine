#include "stdafx.h"
#include "RenderingSystem.h"
#include "EightEngine\Graphics\IRenderableEntity.h"
#include "EightEngine\Graphics\Effect.h"
#include "EightEngine\Graphics\ApiWrapper\Renderer.h"
#include "EightEngine\Actor\Components\DirectionalLightComponent.h"
#include "EightEngine\Graphics\Rendering\SkyRenderingPass.h"
#include "EightEngine\Graphics\Rendering\UIRenderingPass.h"

void LightManager::Init()
{
	m_pLights = std::make_shared<DirectionalLightComponent>();
}
void LightManager::RemoveLights(UINT actorId)
{
	auto pActor = m_pLights->GetActor();
	if (pActor._Get() && pActor._Get()->GetActorID() == actorId)
	{
		Init();
	}
}
void RenderingSystem::Initialize(Renderer &renderer)
{
	m_RenderingPasses.resize(RenderingTypeCount);

	m_RenderingPasses[static_cast<int>(RenderingType::Actor)] = 
		std::make_unique<RenderingPass>();
	m_RenderingPasses[static_cast<int>(RenderingType::Terrain)] =
		std::make_unique<RenderingPass>();
	m_RenderingPasses[static_cast<int>(RenderingType::Sky)] = 
		std::make_unique<SkyRenderingPass>();
	m_RenderingPasses[static_cast<int>(RenderingType::Transparent)] = 
		std::make_unique<RenderingPass>();
	m_RenderingPasses[static_cast<int>(RenderingType::TransparentActor)] =
		std::make_unique<RenderingPass>();
	m_RenderingPasses[static_cast<int>(RenderingType::UI)] = 
		std::make_unique<RenderingPass>();
	auto pTextPass = std::make_unique<UIRenderingPass>();
	pTextPass->Init(renderer);
	m_RenderingPasses[static_cast<int>(RenderingType::Text)] = std::move(pTextPass);


	m_RenderingPasses[static_cast<int>(RenderingType::Actor)]->SetRenderingPipeline(
		renderer.GetEffect("BasicEffect"));
	m_RenderingPasses[static_cast<int>(RenderingType::Terrain)]->SetRenderingPipeline(
		renderer.GetEffect("TerrainEffect"));
	m_RenderingPasses[static_cast<int>(RenderingType::Sky)]->SetRenderingPipeline(
		renderer.GetEffect("SkyEffect"));
	m_RenderingPasses[static_cast<int>(RenderingType::Transparent)]->SetRenderingPipeline(
		renderer.GetEffect("BasicEffect"));
	m_RenderingPasses[static_cast<int>(RenderingType::TransparentActor)]->SetRenderingPipeline(
		renderer.GetEffect("TransparentModelWithoutTexturesEffect"));
	m_RenderingPasses[static_cast<int>(RenderingType::UI)]->SetRenderingPipeline(
		renderer.GetEffect("UserInterfaceEffect"));
	m_RenderingPasses[static_cast<int>(RenderingType::Text)]->SetRenderingPipeline(
		renderer.GetEffect("BasicTextRenderingEffect"));
	
	m_LightManager.Init();
}
#include "EightEngine\Actor\Components\DirectionalLightComponent.h"
void RenderingSystem::Render(Renderer &renderer, std::shared_ptr<GameCamera> &pCamera)
{
	//renderer.SetBackBufferAsRTV();

	auto &pRenderingDeviceContext = renderer.GetRenderingDeviceContext();

	renderer.SetBackBufferAsRenderTarget();

	std::static_pointer_cast<DirectionalLightComponent>(
		m_LightManager.GetLights())->BindLights(pRenderingDeviceContext);
	//LightCBData lightCBData(m_LightManager.GetLights()->GetDirectionalLightData());
	//pRenderingDeviceContext->GetConstantBuffer(LightCBData::NAME)->UpdateBuffer(
	//	&lightCBData, pRenderingDeviceContext);



	CameraCBData cameraCBData(pCamera->GetViewProjMatrix(), pCamera->GetPosition());
	pRenderingDeviceContext->GetConstantBuffer(CameraCBData::NAME)->UpdateBuffer(
		&cameraCBData, pRenderingDeviceContext);

	for (auto &renderingPass : m_RenderingPasses)
	{
		renderingPass->Render(renderer, pCamera);
	}

	if (renderer.IsDebugRenderingOn())
	{
		renderer.GetEffect("TestEffect")->Bind(pRenderingDeviceContext);
		m_RenderingPasses[static_cast<int>(RenderingType::Actor)]->RenderDebug(renderer);
		m_RenderingPasses[static_cast<int>(RenderingType::TransparentActor)]->RenderDebug(renderer);
	}

	for (auto &renderingPass : m_RenderingPasses)
	{
		renderingPass->PostRender();
	}

	renderer.Present();
}