#pragma once

#include "EightEngine\EventManager\IEventData.h"

class OpenExitDoorEventData : public IEventData
{
public:
	static const ULONG s_TYPE;

	OpenExitDoorEventData(bool doorStatus) : m_DoorStatus(doorStatus) { }
	~OpenExitDoorEventData() { }

	bool GetDoorStatus() const { return m_DoorStatus; }

	ULONG VGetEventDataType() const override { return s_TYPE; }
private:
	bool m_DoorStatus;
};