#pragma once

#include "EightEngine\EventManager\IEventData.h"

class CollectItemEventData : public IEventData
{
public:
	static const ULONG s_TYPE;

	CollectItemEventData(UINT itemUsage, UINT numPoints, UINT triggerId, UINT otherActorId) 
		: m_ItemUsage(itemUsage), m_NumPoints(numPoints), m_TriggerId(triggerId), 
		m_OtherActorId(otherActorId) { }
	~CollectItemEventData() { }

	ULONG VGetEventDataType() const override { return s_TYPE; }

	UINT GetNumPoints() const { return m_NumPoints; }
	UINT GetItemUsage() const { return m_ItemUsage; }
	UINT GetTriggerId() const { return m_TriggerId; }
	UINT GetOtherActorId() const { return m_OtherActorId; }
private:
	UINT m_ItemUsage;
	UINT m_NumPoints;
	UINT m_TriggerId;
	UINT m_OtherActorId;
};