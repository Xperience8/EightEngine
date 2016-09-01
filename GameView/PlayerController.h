#pragma once

class EventManager;
#include "EightEngine\EventManager\IEventData.h"

class PlayerController
{
public:
	PlayerController() : m_UnitLookingDirection(0.f, 0.f, 0.f),
		m_UnitMovingDirection(0.f, 0.f, 0.f), m_Jump(false), m_DisableJump(false) { } 
	~PlayerController() { }

	// Only save pointer to event manager
	virtual void RegisterEvents();
	virtual void UnregisterEvents();

	DirectX::XMFLOAT3 &GetUnitLookingDirection() { return m_UnitLookingDirection; }
	DirectX::XMFLOAT3 &GetUnitMovingDirection() { return m_UnitMovingDirection; }
	bool Jump() const { return m_Jump; }
	bool DisabledJumping() const { return m_DisableJump; }
protected:
	void MouseMove(const EventPtr<IEventData> &pEvent);
	void MovementEvent(const EventPtr<IEventData> &pEvent);
	
	void JumpEvent(const EventPtr<IEventData> &pEvent);
	void DisableJumpEvent(const EventPtr<IEventData> &pEvent);


	DirectX::XMFLOAT3 m_UnitMovingDirection;
	DirectX::XMFLOAT3 m_UnitLookingDirection;

	bool m_DisableJump;
	bool m_Jump;
};
