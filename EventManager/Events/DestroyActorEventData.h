#pragma once

#include "EightEngine\EventManager\IEventData.h"

class DestroyActorEventData : public IEventData
{
private:
	UINT m_ActorID;
public:
	static const ULONG s_TYPE;

	DestroyActorEventData(UINT ActorID) : m_ActorID(ActorID) { }
	~DestroyActorEventData() { }

	ULONG VGetEventDataType() const override { return s_TYPE; }

	UINT GetActorID() const { return m_ActorID; }
};