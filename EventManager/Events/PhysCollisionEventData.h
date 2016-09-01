#pragma once

#include "EightEngine\EventManager\IEventData.h"

class Actor;

class PhysCollisionEventData : public IEventData
{
private:
	Actor *m_pActor1;
	Actor *m_pActor2;
public:
	static const ULONG s_TYPE;

	PhysCollisionEventData(Actor *pActor1, Actor *pActor2) 
		: m_pActor1(pActor1), m_pActor2(pActor2) { }
	~PhysCollisionEventData() { }

	Actor *GetFirstActor() { return m_pActor1; }
	Actor *GetSecondActor() { return m_pActor2; }

	ULONG VGetEventDataType() const override { return s_TYPE; }
};