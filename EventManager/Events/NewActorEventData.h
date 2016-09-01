#pragma once

#include "EightEngine\EventManager\IEventData.h"

class Actor;

class NewActorEventData : public IEventData
{
private:
	std::shared_ptr<Actor> m_pActor;
public:
	static const ULONG s_TYPE;

	NewActorEventData(const std::shared_ptr<Actor> &pActor) : m_pActor(pActor) { }
	~NewActorEventData() { }

	std::shared_ptr<Actor> &GetActor() { return m_pActor; }

	ULONG VGetEventDataType() const override { return s_TYPE; }
};