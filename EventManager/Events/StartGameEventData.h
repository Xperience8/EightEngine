#pragma once

#include "EightEngine\EventManager\IEventData.h"

class StartGameEventData : public IEventData
{
private:

public:
	static const ULONG s_TYPE;

	StartGameEventData() { }
	~StartGameEventData() { }

	ULONG VGetEventDataType() const override { return s_TYPE; }
};

class StartNewGameEventData : public IEventData
{
public:
	static const ULONG s_TYPE;

	StartNewGameEventData() { }
	~StartNewGameEventData() { }

	ULONG VGetEventDataType() const override { return s_TYPE; }
};