#pragma once

#include "EightEngine\EventManager\IEventData.h"

// This event has not any data
// It's just empty message, that player loose
class LooseEventData : public IEventData
{
public:
	static const ULONG s_TYPE;

	LooseEventData() { }
	~LooseEventData() { }

	ULONG VGetEventDataType() const override { return s_TYPE; }
};