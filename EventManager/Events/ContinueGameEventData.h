#pragma once

#include "EightEngine\EventManager\IEventData.h"

class ContinueGameEventData : public IEventData
{
public:
	static const ULONG s_TYPE;

	ContinueGameEventData() { }
	~ContinueGameEventData() { }

	ULONG VGetEventDataType() const override { return s_TYPE; }
};