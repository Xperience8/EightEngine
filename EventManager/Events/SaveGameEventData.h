#pragma once

#include "EightEngine\EventManager\IEventData.h"

class SaveGameEventData : public IEventData
{
public:
	static const ULONG s_TYPE;

	SaveGameEventData() { }
	~SaveGameEventData() { }

	ULONG VGetEventDataType() const override { return s_TYPE; }
};