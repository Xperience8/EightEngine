#pragma once

#include "EightEngine\EventManager\IEventData.h"

class DebugCameraEventData : public IEventData
{
private:
	bool m_DebugCameraStatus;
public:
	static const ULONG s_TYPE;

	explicit DebugCameraEventData(bool debugCameraStatus)
		: m_DebugCameraStatus(debugCameraStatus) { }
	~DebugCameraEventData() { }

	bool GetStatus() const { return m_DebugCameraStatus; }

	ULONG VGetEventDataType() const override { return s_TYPE; }
};