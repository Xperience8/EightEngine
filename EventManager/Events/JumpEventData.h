#pragma once

#include "EightEngine\EventManager\IEventData.h"

class JumpEventData : public IEventData
{
public:
	static const ULONG s_TYPE;

	JumpEventData(bool jump) : m_Jump(jump) { }
	~JumpEventData() { }

	ULONG VGetEventDataType() const override { return s_TYPE; }
	
	bool Jump() { return m_Jump; }
private:
	bool m_Jump;
};

class EnableInputEventData : public IEventData
{
private:
	bool m_EnableInput;
public:
	static const ULONG s_TYPE;

	EnableInputEventData(bool enableInput) : m_EnableInput(enableInput) { }
	~EnableInputEventData() { }

	bool IsJumpDisabled() const { return m_EnableInput; }

	ULONG VGetEventDataType() const override { return s_TYPE; }
};

class ChangeCameraEventData : public IEventData
{
public:
	static const ULONG s_TYPE;

	ChangeCameraEventData() { }
	~ChangeCameraEventData() { }

	ULONG VGetEventDataType() const override { return s_TYPE; }
};