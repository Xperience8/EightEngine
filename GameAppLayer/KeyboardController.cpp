#include "stdafx.h"
#include "KeyboardController.h"
#include "EightEngine\EventManager\EventManager.h"
#include "EightEngine\EventManager\Events\MouseMoveEventData.h"
#include "EightEngine\EventManager\Events\PauseGameEventData.h"
#include "EightEngine\EventManager\Events\ConnectToPVDEventData.h"
#include "EightEngine\EventManager\Events\DebugCameraEventData.h"
#include "EightEngine\EventManager\Events\DebugRenderingEventData.h"
#include "EightEngine\EventManager\Events\SaveGameEventData.h"
#include "EightEngine\EventManager\Events\ChangeMenuScreenEventData.h"
#include "EightEngine\EventManager\Events\ExitGameEventData.h"
#include "EightEngine\EventManager\Events\JumpEventData.h"
#include "EightEngine\EventManager\Events\GoToMainMenuEventData.h"
// Keyboard controller

enum KeyCodes
{
	W = 0x57,
	S = 0x53,
	A = 0x41,
	D = 0x44,
	Q = 0x51,
	M = 0x4D,
	Return = 0x0D,
	Space = 0x20,
	Backspace = 0x08,
	Up = 0x26,
	Down = 0x28,
	Num0 = 0x30,
	Num1 = 0x31,
	Num2 = 0x32,
	Num3 = 0x33,
	Num4 = 0x34,
	F4 = 0x73,
	EscapeKey = 0x1B
};
enum MovementDirections
{
	MoveForward = 0x00,
	MoveBackward = 0x01,
	MoveLeft = 0x02,
	MoveRight = 0x03,
};

void KeyboardController::Init()
{
	m_KeyToEventMapping.resize(4, false);
	m_MovingDirection = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	m_IsGamePaused = true;
}
void KeyboardController::KeyDownEvent(UINT keyCode)
{
	switch (keyCode)
	{
	case KeyCodes::Up:
		if (!m_IsGamePaused)
		{
			break;
		}
	case KeyCodes::W:
		m_KeyToEventMapping[MovementDirections::MoveForward] = true;

		m_MovingDirection.z = static_cast<float>(
			m_KeyToEventMapping[MovementDirections::MoveForward] -
			m_KeyToEventMapping[MovementDirections::MoveBackward]);

		g_EventManager.PushEvent(EventPtr<MovementEventData>(m_MovingDirection));

		break;
	case KeyCodes::Down:
		if (!m_IsGamePaused)
		{
			break;
		}
	case KeyCodes::S:
		m_KeyToEventMapping[MovementDirections::MoveBackward] = true;

		m_MovingDirection.z = static_cast<float>(
			m_KeyToEventMapping[MovementDirections::MoveForward] -
			m_KeyToEventMapping[MovementDirections::MoveBackward]);

		g_EventManager.PushEvent(EventPtr<MovementEventData>(m_MovingDirection));

		break;
	case KeyCodes::A:
		m_KeyToEventMapping[MovementDirections::MoveLeft] = true;

		m_MovingDirection.x = static_cast<float>(
			m_KeyToEventMapping[MovementDirections::MoveRight] -
			m_KeyToEventMapping[MovementDirections::MoveLeft]);

		g_EventManager.PushEvent(EventPtr<MovementEventData>(m_MovingDirection));

		break;
	case KeyCodes::D:
		m_KeyToEventMapping[MovementDirections::MoveRight] = true;

		m_MovingDirection.x = static_cast<float>(
			m_KeyToEventMapping[MovementDirections::MoveRight] -
			m_KeyToEventMapping[MovementDirections::MoveLeft]);

		g_EventManager.PushEvent(EventPtr<MovementEventData>(m_MovingDirection));

		break;
	case KeyCodes::Space:
		g_EventManager.PushEvent(EventPtr<JumpEventData>(true));

		break;
	case KeyCodes::Q:
		g_EventManager.PushEvent(EventPtr<PauseGameEventData>());

		break;
	case KeyCodes::Num0:
		// Send connect to physX visual debugger event
		g_EventManager.PushEvent(EventPtr<ConnectToPVDEventData>());

	break;
	case KeyCodes::Num1:
		// Send an event that debug rendering is turned on
		g_EventManager.PushEvent(EventPtr<DebugRenderingEventData>(true));

	break;
	case KeyCodes::Num2:
		// Send an event that debug rendering is turned off
		g_EventManager.PushEvent(EventPtr<DebugRenderingEventData>(false));

	break;
	case KeyCodes::Num3:
		// Send an event that debug camera is turned on
		g_EventManager.PushEvent(EventPtr<DebugCameraEventData>(true));

	break;
	case KeyCodes::Num4:
		// Send an event that debug camera is turned off
		g_EventManager.PushEvent(EventPtr<DebugCameraEventData>(false));

	break;
	case KeyCodes::F4:
		// Send an event that player want to save game
		g_EventManager.PushEvent(EventPtr<SaveGameEventData>());

	break;
	case KeyCodes::Return:
		if (m_IsGamePaused)
		{
			// Send an event that player want to execute actually selected button
			g_EventManager.PushEvent(EventPtr<ActivateGuiElementEventData>());
		}
		break;
	case KeyCodes::Backspace:
		if (m_IsGamePaused)
		{
			// Send an event that player want to go to the previous window(in menu)
			g_EventManager.PushEvent(EventPtr<ChangeMenuScreenEventData>(-1));
		}
		else
		{
			g_EventManager.PushEvent(EventPtr<GoToMainMenuEventData>());
		}
		break;
	case KeyCodes::EscapeKey:
		// Send an event that player want to exit game
		g_EventManager.PushEvent(EventPtr<ExitGameEventData>());

	break;
	}
}
void KeyboardController::KeyUpEvent(UINT keyCode)
{
	switch (keyCode)
	{
	case KeyCodes::Up:
		if (!m_IsGamePaused)
		{
			break;
		}
	case KeyCodes::W:
		m_KeyToEventMapping[MovementDirections::MoveForward] = false;

		m_MovingDirection.z = static_cast<float>(
			m_KeyToEventMapping[MovementDirections::MoveForward] -
			m_KeyToEventMapping[MovementDirections::MoveBackward]);

		g_EventManager.PushEvent(EventPtr<MovementEventData>(m_MovingDirection));

		break;
	case KeyCodes::Down:
		if (!m_IsGamePaused)
		{
			break;
		}
	case KeyCodes::S:
		m_KeyToEventMapping[MovementDirections::MoveBackward] = false;

		m_MovingDirection.z = static_cast<float>(
			m_KeyToEventMapping[MovementDirections::MoveForward] -
			m_KeyToEventMapping[MovementDirections::MoveBackward]);

		g_EventManager.PushEvent(EventPtr<MovementEventData>(m_MovingDirection));

		break;
	case KeyCodes::A:
		m_KeyToEventMapping[MovementDirections::MoveLeft] = false;

		m_MovingDirection.x = static_cast<float>(
			m_KeyToEventMapping[MovementDirections::MoveRight] -
			m_KeyToEventMapping[MovementDirections::MoveLeft]);

		g_EventManager.PushEvent(EventPtr<MovementEventData>(m_MovingDirection));

		break;
	case KeyCodes::D:
		m_KeyToEventMapping[MovementDirections::MoveRight] = false;

		m_MovingDirection.x = static_cast<float>(
			m_KeyToEventMapping[MovementDirections::MoveRight] -
			m_KeyToEventMapping[MovementDirections::MoveLeft]);

		g_EventManager.PushEvent(EventPtr<MovementEventData>(m_MovingDirection));

		break;
	case KeyCodes::Space:
		g_EventManager.PushEvent(EventPtr<JumpEventData>(false));

		break;
	}
}
bool KeyboardController::ProcessInput(UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		KeyDownEvent(static_cast<UINT>(wparam));
		return true;
	case WM_KEYUP:
		KeyUpEvent(static_cast<UINT>(wparam));
		return true;
	}

	return false;
}
