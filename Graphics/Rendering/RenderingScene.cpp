#include "stdafx.h"
#include "RenderingScene.h"
#include "EightEngine\Graphics\ApiWrapper\Renderer.h"
#include "EightEngine\EventManager\EventManager.h"
#include "EightEngine\EventManager\Events\DestroyActorEventData.h"
#include "EightEngine\EventManager\Events\NewActorEventData.h"
#include "EightEngine\Actor\Components\TransformComponent.h"

void RenderingScene::Initialize(Renderer &renderer)
{
	m_pFrustumCullingSystem = std::make_unique<FrustumCullingSystem>();
	m_pRenderingSystem = std::make_unique<RenderingSystem>();

	m_pRenderingSystem->Initialize(renderer);
}
void RenderingScene::RegisterEvents()
{
	g_EventManager.AddListener(EventListener(this, &RenderingScene::AddRenderableEntity),
		NewActorEventData::s_TYPE);
	g_EventManager.AddListener(EventListener(this, &RenderingScene::RemoveRenderableEntity),
		DestroyActorEventData::s_TYPE);
}
void RenderingScene::UnregisterEvents()
{
	g_EventManager.RemoveListener(reinterpret_cast<UINT>(this), NewActorEventData::s_TYPE);
	g_EventManager.RemoveListener(reinterpret_cast<UINT>(this), DestroyActorEventData::s_TYPE);
}
void RenderingScene::AddRenderableEntity(const EventPtr<IEventData> &pEvent)
{
	auto pActor = StaticPointerCast<NewActorEventData>(pEvent)->GetActor();
	EIGHT_ASSERT(pActor, "Actor doesn't exist");

	auto pRenderableEntity = pActor->GetActorComponent<RenderComponent>();
	if (pRenderableEntity)
	{
		if (pRenderableEntity->VGetRenderingType() == RenderingType::NoRendering)
		{
			m_pRenderingSystem->AddLights(std::static_pointer_cast<DirectionalLightComponent>(
				pRenderableEntity));
		}
		else if (pRenderableEntity->VGetRenderingType() == RenderingType::Sky ||
			pRenderableEntity->VGetRenderingType() == RenderingType::Text)
		{
			FrustumCullingModel fcModel;
			fcModel.BoundingBox = DirectX::BoundingBox(DirectX::XMFLOAT3(0.f, 0.f, 0.f),
				DirectX::XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX));
			fcModel.pActor = pActor;
			fcModel.pRenderableEntity = pRenderableEntity.get();
			fcModel.pTransform = pActor->GetActorComponent<TransformComponent>().get();

			m_AllwaysVisibleEntities.push_back(fcModel);
		}
		else {
			m_pFrustumCullingSystem->RegisterActor(pActor);
		}
	}
}
void RenderingScene::RemoveRenderableEntity(const EventPtr<IEventData> &pEvent)
{
	auto pNewEventData = StaticPointerCast<DestroyActorEventData>(pEvent);

	for (auto it = m_AllwaysVisibleEntities.begin();
		it != m_AllwaysVisibleEntities.end(); it++)
	{
		if (pNewEventData->GetActorID() == (*it).pActor->GetActorID())
		{
			m_AllwaysVisibleEntities.erase(it);
			return;
		}
	}

	m_pFrustumCullingSystem->RemoveActor(pNewEventData->GetActorID());

	m_pRenderingSystem->RemoveLights(pNewEventData->GetActorID());
}

/*
UINT RenderingScene::Picking(int sx, int sy, std::shared_ptr<GameCamera> &pCamera)
{
	DirectX::XMMATRIX proj = pCamera->GetProjMatrix();

	float vx = (((2.f * sx) / m_pRenderer->GetScreenWidth()) - 1.f) / proj.r[0].m128_f32[0];
	float vy = (((-2.f * sy) / m_pRenderer->GetScreenHeight()) + 1.f) / proj.r[1].m128_f32[1];

	DirectX::XMVECTOR origin = DirectX::XMVectorZero();
	DirectX::XMVECTOR direction = DirectX::XMVectorSet(vx, vy, 1.f, 0.f);

	DirectX::XMMATRIX invView = DirectX::XMMatrixInverse(&DirectX::XMMatrixDeterminant(
		pCamera->GetViewMatrix()), pCamera->GetViewMatrix());

	origin = DirectX::XMVector3TransformCoord(origin, invView);
	direction = DirectX::XMVector3TransformNormal(direction, invView);

	float minDistance = FLT_MAX;
	UINT actorID = INVALID_ACTOR_ID;

	auto visibleEntities = m_pFrustumCullingSystem->GetVisibleModels();
	for (auto it = visibleEntities.begin(); it != visibleEntities.end(); it++)
	{
		DirectX::BoundingBox bb;
		bb.Extents = (*it)->BoundingBox.Extents;
		DirectX::XMStoreFloat3(&bb.Center, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&(*it)->BoundingBox.Center),
			(*it)->pTransform->GetWorldMatrix().r[3]));
		
		float distance;
		if (bb.Intersects(origin, direction, distance))
		{
			if (minDistance > distance)
			{
				minDistance = distance;
				actorID = (*it)->pActor->GetActorID();
			}
		}
	}

	return actorID;
}
*/
UINT RenderingScene::Picking(DirectX::FXMVECTOR rayOrigin, DirectX::FXMVECTOR rayDirection)
{
	float minDistance = FLT_MAX;
	UINT actorID = INVALID_ACTOR_ID;

	for (auto &entity : m_pFrustumCullingSystem->GetVisibleModels())
	{
		DirectX::BoundingBox bb = entity->BoundingBox;
		bb.Transform(bb, entity->pTransform->GetWorldMatrix());

		float distance;
		if (bb.Intersects(rayOrigin, rayDirection, distance))
		{
			if (minDistance > distance)
			{
				minDistance = distance;
				actorID = entity->pActor->GetActorID();
			}
		}
	}

	return actorID;
}
void RenderingScene::Update(std::shared_ptr<GameCamera> &pCamera, Renderer &renderer)
{
	m_pFrustumCullingSystem->BruteForceFrustumCulling(pCamera);

	auto entities = m_pFrustumCullingSystem->GetVisibleModels();

	for (auto &entity : m_AllwaysVisibleEntities)
	{
		m_pRenderingSystem->AddRenderableEntity(entity);
	}
	for (auto &entity : entities)
	{
		m_pRenderingSystem->AddRenderableEntity(*entity);
	}
}


