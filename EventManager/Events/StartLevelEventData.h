#pragma once

// One level file in which will be:
// Every level attached to specific land(e.g Iceland, Space etc.)
// Every level will has:
//		It's name
//		Requisitions for 3, 2 and 1 star: (think about send this into level config file)
//				Min time
//				Min points
//		Number of stars -> 3, 2, 1 (maybe earned points and time, but probably don't)


#include "EightEngine\EventManager\IEventData.h"

class StartLevelEventData : public IEventData
{
private:

public:
	static const ULONG s_TYPE;

	explicit StartLevelEventData(const std::string &levelName) : m_LevelName(levelName) { }
	~StartLevelEventData() { }

	const std::string &GetLevelName() const { return m_LevelName; }

	ULONG VGetEventDataType() const override { return s_TYPE; }
private:
	std::string m_LevelName;
};