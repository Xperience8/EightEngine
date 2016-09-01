#pragma once

class EventManager;
#include "EightEngine\EventManager\IEventData.h"
class XmlNode;

class Screen
{
public:
	Screen() : m_ScreenWidth(0), m_ScreenHeight(0), m_ScreenLocationX(0), m_ScreenLocationY(0),
		m_Fullscreen(false), m_pScreenHandle(nullptr) { }
	~Screen() { DestroyWindow(m_pScreenHandle); }

	// Create screen from existing screen -> used for editor
	void Init(UINT screenWidth, UINT screenHeight, UINT screenLocationX, 
		UINT screenLocationY, HWND hwnd)
	{ 
		m_ScreenWidth = screenWidth; 
		m_ScreenHeight = screenHeight; 
		m_ScreenLocationX = screenLocationX;
		m_ScreenLocationY = screenLocationY;
		m_pScreenHandle = hwnd; 
	}

	// Create and initialize new screen from config file
	bool Init(const XmlNode &configNode);

	void RegisterEvents();
	void UnregisterEvents();

	// Set new screen width and height
	// Parameters:
	// screenWidth -> screen width int pixels
	// screenHeight -> screen height in pixels
	void Resize(UINT screenWidth, UINT screenHeight)
	{ 
		m_ScreenWidth = screenWidth; 
		m_ScreenHeight = screenHeight; 
	}

	// If screen(or window) is minimized this will(should) be called and it perform flashing
	void FlashScreen(const EventPtr<IEventData> &pEvent);

	// Get screen width in pixels
	UINT GetScreenWidth() const { return m_ScreenWidth; }
	
	// Get screen height in pixels
	UINT GetScreenHeight() const { return m_ScreenHeight; }
	
	// Get screen handle
	// This is used by renderer, which attach it as render target output
	HWND GetScreenHandle() const { return m_pScreenHandle; }

	// Return screens aspect ratio
	float GetScreenAspectRatio() const 
	{ 
		return static_cast<float>(m_ScreenWidth) / static_cast<float>(m_ScreenHeight); 
	}

	// Get screen center point in pixels
	// This is used for computing mouse coords relative to screen
	POINT GetScreenCenter() const
	{
		POINT p;
		p.x = static_cast<LONG>(0.5f * m_ScreenWidth);
		p.y = static_cast<LONG>(0.5f * m_ScreenHeight);
		ClientToScreen(m_pScreenHandle, &p);

		p.x += m_ScreenLocationX;
		p.y += m_ScreenLocationY;

		return p;
	}

	RECT GetScreenRect() const
	{
		RECT rect;
		GetWindowRect(m_pScreenHandle, &rect);
		
		rect.left += m_ScreenLocationX;
		rect.right += m_ScreenLocationX;
		rect.bottom += m_ScreenLocationY;
		rect.top += m_ScreenLocationY;
		
		return rect;
	}


	// Actually only changes fullscreen settings
	void ChangeScreenSettings(bool fullscreen);
	
	// Convert point from screen space to NDC space
	// Screen space is from 0 to screenWidth, 0 to screenHeight
	// NDC space is from -1.f to 1.f it is independent of screen size
	// Z coord stays same
	DirectX::XMFLOAT3 PointToNDC(const DirectX::XMFLOAT3 &point)
	{
		return DirectX::XMFLOAT3(
			2.f * (point.x / static_cast<float>(m_ScreenWidth)) - 1.f,
			-2.f * (point.y / static_cast<float>(m_ScreenHeight)) + 1.f,
			point.z);
	}

	// Convert point from NDC space to screen space
	// Screen space is from 0 to screenWidth, 0 to screenHeight
	// NDC space is from -1.f to 1.f it is independent of screen size
	// Z coord stays same
	DirectX::XMFLOAT3 PointFromNDCToPx(const DirectX::XMFLOAT3 &point)
	{
		return DirectX::XMFLOAT3(
			(point.x + 1.f) * 0.5f * m_ScreenWidth,
			(point.y - 1.f) * -0.5f * m_ScreenHeight,
			point.z);
	}
private:
	HWND m_pScreenHandle;
	UINT m_ScreenWidth;
	UINT m_ScreenHeight;
	UINT m_ScreenLocationX;
	UINT m_ScreenLocationY;

	bool m_Fullscreen;

	bool RegisterScreen(const std::wstring &ScreenName);
	bool CreateScreen(const std::wstring &ScreenName);
	void ActivateScreen();
};