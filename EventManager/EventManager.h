#pragma once

#include "IEventData.h"
#include "EventListener.h"

class EventManager
{
public:
	EventManager() : m_ActiveQueue(0) { }
	~EventManager() { }

	// Create and initialize allocator
	void StartUp();

	// Pending events are discarded and every event listener is released
	void ShutDown();

	// Add new evenet listener so he can catch event of "eventType" type
	void AddListener(EventListener &eventListener, ULONG eventType);

	// Remove existing event listener
	void RemoveListener(UINT eventListenerId, ULONG eventType);

	// Event is inserted into queue and it will be processed in next Update call
	void PushEvent(const EventPtr<IEventData> &pEventData)
	{
		EIGHT_ASSERT(pEventData, "You can not push empty event");
		m_EventQueues[m_ActiveQueue].push_back(pEventData);
	}

	// Event is immediately send to every listener
	void TriggerEvent(const EventPtr<IEventData> &pEventData)
	{
		EIGHT_ASSERT(pEventData, "You can not trigger empty event");

		for (auto &eventListener : m_EventListenersMap[pEventData->VGetEventDataType()])
		{
			eventListener(pEventData);
		}
	}

	// Events from queue are send to their listeners
	void Update();

private:
	// There are two queues because when you processed events, they can trigger another events
	// and from this you can create infinite loop
	// If there are two queues, pin pong scheme is achieved, if you process one queue
	// new events are put into second queue and after processing their roles are reversed
	static const UINT MAX_QUEUES = 2;

	// Event listeners are called with specific events
	// Key is event type
	std::unordered_map<ULONG, std::list<EventListener> > m_EventListenersMap;

	// Events which were pushed
	std::list<EventPtr<IEventData> > m_EventQueues[MAX_QUEUES];

	// It is used for ping pong scheme
	// One queues process events, the other collects new events, then their roles are reversed
	UINT m_ActiveQueue;
};

extern EventManager g_EventManager;
