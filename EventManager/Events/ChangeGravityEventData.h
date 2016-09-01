#pragma once

#include "EightEngine\EventManager\IEventData.h"

class ChangeGravityEventData : public IEventData
{
public:
	static const ULONG s_TYPE;

	ChangeGravityEventData(bool status, UINT actorId) : m_ActorId(actorId), m_Status(status) { }
	~ChangeGravityEventData() { }

	UINT GetActorId() const { return m_ActorId; }
	bool GetStatus() const { return m_Status; }

	ULONG VGetEventDataType() const override { return s_TYPE; }
private:
	UINT m_ActorId;
	bool m_Status;
};