#include "stdafx.h"
#include "PlayerView.h"
#include "TopCamera.h"
#include "EightEngine\Graphics\ApiWrapper\Renderer.h"
#include "EightEngine\Actor\Components\TransformComponent.h"
#include "EightEngine\EventManager\EventManager.h"
#include "EightEngine\EventManager\Events\DebugCameraEventData.h"

void PlayerView::VOnRestore()
{
	m_pCamera = std::make_shared<TopCamera>();
	m_pCamera->SetOffsetFromActor(DirectX::XMVectorSet(0.f, 3.f, 0.f, 0.f));

	m_pController = std::make_unique<PlayerController>();
}

void PlayerView::VUpdate(float deltaTime)
{	
	auto &lookingDir = m_pController->GetUnitLookingDirection();
	if (lookingDir.x)
	{
		m_pCamera->Yaw(DirectX::XMConvertToRadians(lookingDir.x));
	}
	if (lookingDir.y)
	{
		m_pCamera->Pitch(DirectX::XMConvertToRadians(lookingDir.y));
	}

	// If debug camera is on we just moving with camera(only) and actor is ingored
	if (m_DebugCamera)
	{
		const float speed = 2.f;
		const DirectX::XMFLOAT3 &movingDir = m_pController->GetUnitMovingDirection();

		DirectX::XMVECTOR cameraPosition = m_pCamera->GetPosition();

		DirectX::XMVECTOR offsetInLookDirection = DirectX::XMVectorMultiply(
			DirectX::XMVectorReplicate(deltaTime * speed * movingDir.z),
			m_pCamera->GetLookingDirection());

		DirectX::XMVECTOR offsetInRightDirection = DirectX::XMVectorMultiply(
			DirectX::XMVectorReplicate(deltaTime * speed * movingDir.x),
			m_pCamera->GetRightDirection());

		cameraPosition = DirectX::XMVectorAdd(cameraPosition, offsetInLookDirection);
		cameraPosition = DirectX::XMVectorAdd(cameraPosition, offsetInRightDirection);

		m_pCamera->SetPosition(cameraPosition);
	}
	else {
		auto pTransformComponent = m_pActor._Get()->GetActorComponent<TransformComponent>();
		
		DirectX::XMMATRIX worldMatrix = pTransformComponent->GetWorldMatrix();

		m_pCamera->VUpdate(worldMatrix.r[3]);
	}

	//lookingDir.x = 0.f;
	//lookingDir.y = 0.f;
}
void PlayerView::VRender()
{
	g_Renderer.PushCamera(m_pCamera);
}

void PlayerView::ChangeDebugCameraStatus(const EventPtr<IEventData> &pEvent)
{
	auto pEventData = StaticPointerCast<DebugCameraEventData>(pEvent);
	m_DebugCamera = pEventData->GetStatus();
}
void PlayerView::VRegisterEvents()
{
	m_pController->RegisterEvents();
	
	g_EventManager.AddListener(EventListener(this, &PlayerView::ChangeDebugCameraStatus),
		DebugCameraEventData::s_TYPE);
}
void PlayerView::VUnregisterEvents()
{
	m_pController->UnregisterEvents();

	g_EventManager.RemoveListener(reinterpret_cast<UINT>(this),
		DebugCameraEventData::s_TYPE);
}