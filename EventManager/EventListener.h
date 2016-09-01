#pragma once

#include "IEventData.h"

class EventListener
{
private:
	std::function<void(const EventPtr<IEventData> &)> m_pFunction;
	UINT m_ID;
public:
	template <typename Object, typename Function>
	EventListener(Object object, Function function) : m_ID(reinterpret_cast<UINT>(object)),
		m_pFunction(std::bind(function, object, std::placeholders::_1))  { }
	~EventListener() { }

	void operator()(const EventPtr<IEventData> &pEventData) { m_pFunction(pEventData); }

	friend bool operator==(const EventListener &l1, const EventListener &l2) 
	{ 
		return (l1.m_ID == l2.m_ID); 
	}
	friend bool operator==(const EventListener &l1, UINT EventListenerID) 
	{ 
		return (l1.m_ID == EventListenerID); 
	}
};