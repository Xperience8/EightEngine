#include "stdafx.h"
#include "Screen.h"
#include "Resource.h"
#include "EightEngine\GameAppLayer\InputManager.h"
#include "EightEngine\EventManager\EventManager.h"
#include "EightEngine\EventManager\Events\MinimizeGameEventData.h"
#include "EightEngine\Xml\XmlNode.h"

bool Screen::Init(const XmlNode &configNode)
{
	auto resolutionNode = configNode.GetChildrenNode("Resolution");
	m_ScreenWidth = std::stoi(resolutionNode.GetAttribute("screenWidth").GetValue());
	m_ScreenHeight = std::stoi(resolutionNode.GetAttribute("screenHeight").GetValue());

	auto fullscreenNode = configNode.GetChildrenNode("Fullscreen");
	m_Fullscreen = std::stoi(fullscreenNode.GetAttribute().GetValue()) > 0;

	auto screenNode = configNode.GetChildrenNode("Screen");
	std::wstring screenName = ToWstring(screenNode.GetAttribute().GetValue());

	if (!RegisterScreen(screenName))
	{
		EIGHT_ERROR("Register screen in windows failed");
		return false;
	}
	if (!CreateScreen(screenName))
	{
		EIGHT_ERROR("Creating screen failed");
		return false;
	}

	ActivateScreen();

	EIGHT_LOG("Screen was successfuly registered, created, initialized and activated");
	return true;
}

bool Screen::RegisterScreen(const std::wstring &screenName)
{
	// Fill description with information about screen
	WNDCLASSEX wc;
	wc.cbSize = sizeof(wc);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, MAKEINTRESOURCE(107));
	wc.hIconSm = LoadIcon(nullptr, MAKEINTRESOURCE(108));
	wc.hInstance = GetModuleHandle(nullptr);
	wc.lpfnWndProc = InputManager::GameWndProc;
	wc.lpszClassName = screenName.c_str();
	wc.lpszMenuName = nullptr;
	wc.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;

	if (RegisterClassEx(&wc))
	{
		return true;
	}
	return false;
}
bool Screen::CreateScreen(const std::wstring &screenName)
{
	if (m_Fullscreen)
	{
		ChangeScreenSettings(m_Fullscreen);
	}

	m_pScreenHandle = CreateWindowEx(WS_EX_APPWINDOW, screenName.c_str(), 
		screenName.c_str(), WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		0, 0, m_ScreenWidth, m_ScreenHeight, nullptr, nullptr, nullptr, nullptr);

	return m_pScreenHandle != nullptr;
}
void Screen::ChangeScreenSettings(bool fullscreen)
{
	// If you want to fullscreen mode and it was not allready in use
	if (fullscreen)
	{
		if (m_Fullscreen)
		{
			DEVMODE dmScreenSettings;
			memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
			dmScreenSettings.dmSize = sizeof(dmScreenSettings);
			dmScreenSettings.dmPelsWidth = (unsigned long)m_ScreenWidth;
			dmScreenSettings.dmPelsHeight = (unsigned long)m_ScreenHeight;
			dmScreenSettings.dmBitsPerPel = 32;
			dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

			m_Fullscreen = true;
		}
	}
	else if (m_Fullscreen)
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)m_ScreenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)m_ScreenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, 0);

		m_Fullscreen = false;
	}
}
void Screen::ActivateScreen()
{
	ShowWindow(m_pScreenHandle, SW_SHOW);
	SetFocus(m_pScreenHandle);
	SetForegroundWindow(m_pScreenHandle);
}
void Screen::FlashScreen(const EventPtr<IEventData> &pEvent)
{
	if (IsIconic(m_pScreenHandle))
	{
		FLASHWINFO flashInfo;
		flashInfo.cbSize = sizeof(FLASHWINFO);
		flashInfo.dwFlags = FLASHW_ALL | FLASHW_TIMER;
		flashInfo.dwTimeout = 1000;
		flashInfo.hwnd = m_pScreenHandle;
		flashInfo.uCount = 0;
		
		FlashWindowEx(&flashInfo);
		
		flashInfo.dwFlags = FLASHW_STOP;

		MSG msg = { 0 };
		while (true)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_SYSCOMMAND && msg.wParam == SC_CLOSE)
					break;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else if (!IsIconic(m_pScreenHandle))
			{
				FlashWindowEx(&flashInfo);
				break;
			}
		}
	}
}
void Screen::RegisterEvents()
{
	g_EventManager.AddListener(EventListener(this, &Screen::FlashScreen),
		MinimizeGameEventData::s_TYPE);
}
void Screen::UnregisterEvents()
{
	g_EventManager.RemoveListener(reinterpret_cast<UINT>(this),
		MinimizeGameEventData::s_TYPE);
}
