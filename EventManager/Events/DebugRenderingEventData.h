#pragma once

#include "EightEngine\EventManager\IEventData.h"

class DebugRenderingEventData : public IEventData
{
private:
	bool m_DebugRenderingStatus;
public:
	static const ULONG s_TYPE;

	DebugRenderingEventData(bool debugRenderingStatus) 
		: m_DebugRenderingStatus(debugRenderingStatus) { }
	~DebugRenderingEventData() { }

	bool GetStatus() const { return m_DebugRenderingStatus; }

	ULONG VGetEventDataType() const override { return s_TYPE; }
};
