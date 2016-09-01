#pragma once

#include "EightEngine\EventManager\IEventData.h"

class PauseGameEventData : public IEventData
{
private:

public:
	static const ULONG s_TYPE;

	PauseGameEventData() { }
	~PauseGameEventData() { }

	ULONG VGetEventDataType() const override { return s_TYPE; }
};