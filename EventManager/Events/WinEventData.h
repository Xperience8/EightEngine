#pragma once

#include "EightEngine\EventManager\IEventData.h"

// This event has not any data
// It's just empty message, that player complete level
class WinEventData : public IEventData
{
public:
	static const ULONG s_TYPE;

	explicit WinEventData(UINT status = 0) : m_Status(status)  { }
	~WinEventData() { }

	UINT GetStatus() const { return m_Status; }

	ULONG VGetEventDataType() const override { return s_TYPE; }
private:
	UINT m_Status;
};

class ComputePlayerRewardEventData : public IEventData
{
public:
	static const ULONG s_TYPE;

	ComputePlayerRewardEventData() { }
	~ComputePlayerRewardEventData() { }

	ULONG VGetEventDataType() const override { return s_TYPE; }
};