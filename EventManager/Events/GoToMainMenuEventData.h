#pragma once

#include "EightEngine\EventManager\IEventData.h"

class GoToMainMenuEventData : public IEventData
{
public:
	static const ULONG s_TYPE;

	GoToMainMenuEventData() { }
	~GoToMainMenuEventData() { }

	ULONG VGetEventDataType() const override { return s_TYPE; }
};