#pragma once

#include "EightEngine\EventManager\IEventData.h"

class GameCamera;

class DragInitEventData : public IEventData
{
private:
	UINT m_ActorID;
	std::weak_ptr<GameCamera> m_pCamera;
public:
	static const ULONG s_TYPE;

	DragInitEventData(UINT ActorID) : m_ActorID(ActorID) { }
	DragInitEventData(UINT ActorID, std::weak_ptr<GameCamera> &pCamera) 
		: m_ActorID(ActorID), m_pCamera(pCamera) { }
	~DragInitEventData() { }

	ULONG VGetEventDataType() const override { return s_TYPE; }

	UINT GetActorID() const { return m_ActorID; }
	std::weak_ptr<GameCamera> &GetCamera() { return m_pCamera; }
};