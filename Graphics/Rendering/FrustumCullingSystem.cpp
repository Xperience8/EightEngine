#include "stdafx.h"
#include "FrustumCullingSystem.h"
#include "EightEngine\Graphics\IRenderableEntity.h"
#include "EightEngine\Actor\Components\TransformComponent.h"
#include "EightEngine\Actor\Components\ModelRenderComponent.h"
#include "EightEngine\GameView\GameCamera.h"

bool FrustumCullingSystem::AABBFrustumIntersection(const FrustumCullingModel &model, std::shared_ptr<GameCamera> &pCamera)
{
	DirectX::BoundingBox box = model.BoundingBox;

	DirectX::XMVECTOR center = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&model.BoundingBox.Center), model.pTransform->GetWorldMatrix().r[3]);
	DirectX::XMVECTOR extents = DirectX::XMLoadFloat3(&model.BoundingBox.Extents);

	const Frustum &frustum = pCamera->GetFrustum();

	for (UINT i = 0; i < 6; i++)
	{
		DirectX::XMVECTOR plane = DirectX::XMLoadFloat4(&frustum.Planes[i]);
		DirectX::XMVECTOR absPlane = DirectX::XMLoadFloat4(&frustum.AbsPlanes[i]);

		float r = DirectX::XMVectorGetX(DirectX::XMVector3Dot(plane, center));
		float s = DirectX::XMVectorGetX(DirectX::XMVector3Dot(absPlane, extents));

		if ((s + r) < -plane.m128_f32[3])
			return false;
	}
	return true;
}
void FrustumCullingSystem::RegisterActor(std::shared_ptr<Actor> &pActor)
{
	auto pTransformComponent = pActor->GetActorComponent<TransformComponent>();
	auto pRenderComponent = pActor->GetActorComponent<ModelRenderComponent>();

	EIGHT_ASSERT(pTransformComponent, "This Actor doesn't has Transform Component, i.e it can not be inserted into culling procedure");
	EIGHT_ASSERT(pRenderComponent, "This Actor doesn't has Render Component, i.e. it can not be rendered, no culling is needed");

	FrustumCullingModel model;
	model.BoundingBox = pRenderComponent->GetBoundingBox();
	model.pTransform = pTransformComponent.get();
	model.pRenderableEntity = pRenderComponent.get();
	model.pActor = pActor;

	m_Models.push_back(model);

	EIGHT_LOG("Actor was successfully added into frustum culling subsystem");
}
void FrustumCullingSystem::RemoveActor(UINT ActorID)
{
	for (auto it = m_Models.begin(); it != m_Models.end(); it++)
	{
		if ((it)->pActor->GetActorID() == ActorID)
		{
			std::iter_swap(it, m_Models.end() - 1);
			m_Models.pop_back();
			break;
		}
	}
}