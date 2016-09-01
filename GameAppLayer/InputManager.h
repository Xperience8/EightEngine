#pragma once

class Screen;
#include "EightEngine\EventManager\IEventData.h"

#include "KeyboardController.h"
#include "MouseController.h"

class InputManager
{
private:
	MouseController m_MouseController;
	KeyboardController m_KeyboardController;
	
	MSG m_InputMessage;

	void SetMouseCursorVisibility(const EventPtr<IEventData> &pEvent);

public:
	InputManager() { }
	~InputManager() { }

	void StartUp(const std::shared_ptr<Screen> &pScreen);
	void ShutDown();

	void Update();

	static LRESULT CALLBACK GameWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};

extern InputManager g_InputManager;