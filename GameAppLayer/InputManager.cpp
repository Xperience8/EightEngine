#include "stdafx.h"
#include "InputManager.h"
#include "EightEngine\GameAppLayer\Screen.h"
#include "EightEngine\EventManager\EventManager.h"
#include "EightEngine\EventManager\Events\ExitGameEventData.h"
#include "EightEngine\EventManager\Events\MinimizeGameEventData.h"
#include "EightEngine\EventManager\Events\ChangeMenuScreenEventData.h"
#include "EightEngine\EventManager\Events\MouseMoveEventData.h"

InputManager g_InputManager;

LRESULT CALLBACK InputManager::GameWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (g_InputManager.m_MouseController.ProcessInput(msg, wparam, lparam))
	{
		return 0;
	}
	else if (g_InputManager.m_KeyboardController.ProcessInput(msg, wparam, lparam))
	{
		return 0;
	}
	else
	{
		switch (msg)
		{
		case WM_SIZE:
		if (wparam == SIZE_MINIMIZED)
		{
			// Send an event that window is minimized so flashing can come up
			EventPtr<MinimizeGameEventData> pEvent;
			g_EventManager.PushEvent(pEvent);
		}
		return 0;
		case WM_CLOSE:
		{
			// Send an event that player want to exit game
			EventPtr<ExitGameEventData> pEvent;
			g_EventManager.PushEvent(pEvent);
		}
		return 0;
		}
	}

	// Event was not handled(this app doesn't care about him)
	return DefWindowProc(hwnd, msg, wparam, lparam);
}
void InputManager::StartUp(const std::shared_ptr<Screen> &pScreen)
{
	m_KeyboardController.Init();
	m_MouseController.Init(pScreen);

	g_EventManager.AddListener(EventListener(this, &InputManager::SetMouseCursorVisibility),
		ShowMouseCursorEventData::s_TYPE);
}
void InputManager::ShutDown()
{
	g_EventManager.RemoveListener(reinterpret_cast<UINT>(this),
		ShowMouseCursorEventData::s_TYPE);

	m_MouseController.Destroy();
	m_KeyboardController.Destroy();
}
void InputManager::Update()
{
	// Process every input message from OS
	while (PeekMessage(&m_InputMessage, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&m_InputMessage);
		DispatchMessage(&m_InputMessage);
	}
}
void InputManager::SetMouseCursorVisibility(const EventPtr<IEventData> &pEvent)
{
	bool cursorVisibility = StaticPointerCast<ShowMouseCursorEventData>(
		pEvent)->GetCursorVisibility();

	m_KeyboardController.SetGameStatus(cursorVisibility);
	m_MouseController.ShowMouseCursor(cursorVisibility);

	g_EventManager.PushEvent(EventPtr<MouseMoveEventData>(0.f, 0.f));
	g_EventManager.PushEvent(EventPtr<MovementEventData>(DirectX::XMFLOAT3(0.f, 0.f, 0.f)));
}