#pragma once

#include "EightEngine\EventManager\IEventData.h"

class MinimizeGameEventData : public IEventData
{
public:
	static const ULONG s_TYPE;

	MinimizeGameEventData() { }
	~MinimizeGameEventData() { }

	ULONG VGetEventDataType() const override { return s_TYPE; }
};