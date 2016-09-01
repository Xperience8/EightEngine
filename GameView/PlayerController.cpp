#include "stdafx.h"
#include "PlayerController.h"
#include "EightEngine\EventManager\EventManager.h"
#include "EightEngine\EventManager\Events\MouseMoveEventData.h"
#include "EightEngine\EventManager\Events\JumpEventData.h"

void PlayerController::RegisterEvents()
{
	g_EventManager.AddListener(EventListener(this, &PlayerController::MouseMove),
		MouseMoveEventData::s_TYPE);
	g_EventManager.AddListener(EventListener(this, &PlayerController::MovementEvent),
		MovementEventData::s_TYPE);
	g_EventManager.AddListener(EventListener(this, &PlayerController::JumpEvent),
		JumpEventData::s_TYPE);
	g_EventManager.AddListener(EventListener(this, &PlayerController::DisableJumpEvent),
		EnableInputEventData::s_TYPE);

	g_EventManager.PushEvent(EventPtr<ShowMouseCursorEventData>(false));
}
void PlayerController::UnregisterEvents()
{
	g_EventManager.RemoveListener(reinterpret_cast<UINT>(this), EnableInputEventData::s_TYPE);
	g_EventManager.RemoveListener(reinterpret_cast<UINT>(this), JumpEventData::s_TYPE);
	g_EventManager.RemoveListener(reinterpret_cast<UINT>(this), MouseMoveEventData::s_TYPE);
	g_EventManager.RemoveListener(reinterpret_cast<UINT>(this), MovementEventData::s_TYPE);
}
void PlayerController::MouseMove(const EventPtr<IEventData> &pEvent)
{
	auto pEventData = StaticPointerCast<MouseMoveEventData>(pEvent);
	m_UnitLookingDirection.x = pEventData->GetCurrentMousePosX();
	m_UnitLookingDirection.y = pEventData->GetCurrentMousePosY();
}
void PlayerController::MovementEvent(const EventPtr<IEventData> &pEvent)
{
	auto pEventData = StaticPointerCast<MovementEventData>(pEvent);
	m_UnitMovingDirection = pEventData->GetMovementDirection();

	DirectX::XMStoreFloat3(&m_UnitMovingDirection, DirectX::XMVector3Normalize(
		DirectX::XMLoadFloat3(&m_UnitMovingDirection)));
}
void PlayerController::JumpEvent(const EventPtr<IEventData> &pEvent)
{
	auto pEventData = StaticPointerCast<JumpEventData>(pEvent);

	m_Jump = pEventData->Jump();
}
void PlayerController::DisableJumpEvent(const EventPtr<IEventData> &pEvent)
{
	auto pEventData = StaticPointerCast<EnableInputEventData>(pEvent);
	
	m_DisableJump = pEventData->IsJumpDisabled();
	m_Jump = false;
}