#pragma once

#include "EightEngine\EventManager\IEventData.h"

class MouseMoveEventData : public IEventData
{
private:
	float m_CurrentMousePosX;
	float m_CurrentMousePosY;
public:
	static const ULONG s_TYPE;

	MouseMoveEventData(float currentPosX, float currentPosY) 
		: m_CurrentMousePosX(currentPosX), m_CurrentMousePosY(currentPosY) { }
	~MouseMoveEventData() { }

	float GetCurrentMousePosX() const { return m_CurrentMousePosX; }
	float GetCurrentMousePosY() const { return m_CurrentMousePosY; }

	ULONG VGetEventDataType() const override { return s_TYPE; }
};

class ShowMouseCursorEventData : public IEventData
{
private:
	bool m_IsVisible;
public:
	static const ULONG s_TYPE;

	explicit ShowMouseCursorEventData(bool isVisible) : m_IsVisible(isVisible) { }
	~ShowMouseCursorEventData() { }

	bool GetCursorVisibility() const { return m_IsVisible; }

	ULONG VGetEventDataType() const override { return s_TYPE; }
};

class MovementEventData : public IEventData
{
private:
	DirectX::XMFLOAT3 m_MovementDirection;
public:
	static const ULONG s_TYPE;

	explicit MovementEventData(DirectX::XMFLOAT3 &movementDirection)
		: m_MovementDirection(movementDirection) { }
	~MovementEventData() { }

	const DirectX::XMFLOAT3 &GetMovementDirection() const { return m_MovementDirection; }

	ULONG VGetEventDataType() const override { return s_TYPE; }
};


class RightMouseButtonEventData : public IEventData
{
private:
	bool m_Status;
public:
	static const ULONG s_TYPE;

	explicit RightMouseButtonEventData(bool status) : m_Status(status) { }
	~RightMouseButtonEventData() { }
	
	bool GetStatus() const { return m_Status; }

	ULONG VGetEventDataType() const override { return s_TYPE; }
};

class LeftMouseButtonEventData : public IEventData
{
private:
	UINT m_ScreenCoordX;
	UINT m_ScreenCoordY;
	bool m_Status;
public:
	static const ULONG s_TYPE;

	LeftMouseButtonEventData(UINT x, UINT y, bool status) : m_Status(status),
		m_ScreenCoordX(x), m_ScreenCoordY(y) { }
	~LeftMouseButtonEventData() { }

	bool GetStatus() const { return m_Status; }
	UINT GetScreenPosX() const { return m_ScreenCoordX; }
	UINT GetScreenPosY() const { return m_ScreenCoordY; }

	ULONG VGetEventDataType() const override { return s_TYPE; }
};