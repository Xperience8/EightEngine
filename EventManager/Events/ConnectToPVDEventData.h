#pragma once

#include "EightEngine\EventManager\IEventData.h"

class ConnectToPVDEventData : public IEventData
{
public:
	static const ULONG s_TYPE;

	ConnectToPVDEventData() { }
	~ConnectToPVDEventData() { }

	ULONG VGetEventDataType() const override { return s_TYPE; }
};