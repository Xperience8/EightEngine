#include "stdafx.h"
#include "MouseController.h"
#include "EightEngine\EventManager\EventManager.h"
#include "EightEngine\GameAppLayer\Screen.h"
#include "EightEngine\EventManager\Events\MouseMoveEventData.h"
#include "EightEngine\EventManager\Events\ChangeMenuScreenEventData.h"

MouseController::MouseController() : m_PercentOfNew(0.5f), m_PercentOfOld(0.5f),
	m_DeltaX(0.f), m_DeltaY(0.f), m_IsCursorVisible(true)
{

}

void MouseController::Init(const std::shared_ptr<Screen> &pScreen)
{
	m_DeltaX = 0.f;
	m_DeltaY = 0.f;

	m_IsCursorVisible = true;

	RAWINPUTDEVICE rid;
	rid.dwFlags = RIDEV_INPUTSINK;
	rid.usUsage = 0x02;
	rid.usUsagePage = 0x01;
	rid.hwndTarget = pScreen->GetScreenHandle();
	RegisterRawInputDevices(&rid, 1, sizeof(rid));

	RECT rect = pScreen->GetScreenRect();
	ClipCursor(&rect);
}
void MouseController::Destroy()
{
	m_DeltaX = 0.f;
	m_DeltaY = 0.f;

	m_IsCursorVisible = true;
}
void MouseController::MouseMove(int mousePosX, int mousePosY)
{
	if (m_IsCursorVisible)
	{
		POINT point;
		GetCursorPos(&point);

		g_EventManager.PushEvent(EventPtr<MouseMoveEventData>(
			static_cast<float>(point.x), static_cast<float>(point.y)));
	}
	else
	{
		m_DeltaX = m_DeltaX * m_PercentOfOld + mousePosX * m_PercentOfNew;
		m_DeltaY = m_DeltaY * m_PercentOfOld + mousePosY * m_PercentOfNew;

		g_EventManager.TriggerEvent(EventPtr<MouseMoveEventData>(m_DeltaX, m_DeltaY));
	}
}
void MouseController::ShowMouseCursor(bool cursorIsVisible)
{
	m_IsCursorVisible = cursorIsVisible;

	if (m_IsCursorVisible)
	{
		while (ShowCursor(true) < 0);
	}
	else
	{
		while (ShowCursor(false) > -1);
	}
}
bool MouseController::ProcessInput(UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_INPUT:
	{
		UINT dwSize = sizeof(RAWINPUT);
		static BYTE lpb[sizeof(RAWINPUT)];

		GetRawInputData((HRAWINPUT)lparam, RID_INPUT,
			lpb, &dwSize, sizeof(RAWINPUTHEADER));

		RAWINPUT* raw = (RAWINPUT*)lpb;

		if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			int xPosRelative = raw->data.mouse.lLastX;
			int yPosRelative = raw->data.mouse.lLastY;

			MouseMove(xPosRelative, yPosRelative);
		}
	}
		return true;
	case WM_LBUTTONDOWN:
	{
		UINT posX = GET_X_LPARAM(lparam);
		UINT posY = GET_Y_LPARAM(lparam);

		g_EventManager.PushEvent(EventPtr<LeftMouseButtonEventData>(posX, posY, true));
		g_EventManager.PushEvent(EventPtr<ActivateGuiElementByMouseEventData>(posX, posY));
	}
	return true;
	case WM_LBUTTONUP:
		g_EventManager.PushEvent(EventPtr<LeftMouseButtonEventData>(0, 0, false));
		return true;
	case WM_RBUTTONDOWN:
		g_EventManager.PushEvent(EventPtr<RightMouseButtonEventData>(true));
		return true;
	case WM_RBUTTONUP:
		g_EventManager.PushEvent(EventPtr<RightMouseButtonEventData>(false));
		return true;
	}

	return false;
}