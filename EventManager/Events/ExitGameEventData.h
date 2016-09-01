#pragma once

#include "EightEngine\EventManager\IEventData.h"

class ExitGameEventData : public IEventData
{
private:

public:
	static const ULONG s_TYPE;

	ExitGameEventData() { }
	~ExitGameEventData() { }

	ULONG VGetEventDataType() const override { return s_TYPE; }
};