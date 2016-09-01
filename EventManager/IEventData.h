#pragma once

#include "EightEngine\Memory\EventAllocator.h"

class IEventData
{
public:
	IEventData() { }
	virtual ~IEventData() { }

	virtual ULONG VGetEventDataType() const = 0;
};