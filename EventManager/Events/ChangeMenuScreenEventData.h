#pragma once

#include "EightEngine\EventManager\IEventData.h"

class ChangeMenuScreenEventData : public IEventData
{
private:
	UINT m_NewMenuScreen;
public:
	static const ULONG s_TYPE;

	ChangeMenuScreenEventData(UINT newMenuScreen) : m_NewMenuScreen(newMenuScreen) { }
	//ChangeMenuScreenEventData() { }
	~ChangeMenuScreenEventData() { }

	ULONG VGetEventDataType() const override { return s_TYPE; }

	UINT GetNewMenuScreenId() const { return m_NewMenuScreen; }
};

class ActivateGuiElementEventData : public IEventData
{
public:
	static const ULONG s_TYPE;

	ActivateGuiElementEventData() { }
	~ActivateGuiElementEventData() { }

	ULONG VGetEventDataType() const override { return s_TYPE; }
};

class ActivateGuiElementByMouseEventData : public IEventData
{
private:
	UINT m_PositionX;
	UINT m_PositionY;
public:
	static const ULONG s_TYPE;

	ActivateGuiElementByMouseEventData(UINT posX, UINT posY)
		: m_PositionX(posX), m_PositionY(posY) { }
	~ActivateGuiElementByMouseEventData() { }

	UINT GetPositionX() const { return m_PositionX; }
	UINT GetPositionY() const { return m_PositionY; }

	ULONG VGetEventDataType() const override { return s_TYPE; }
};