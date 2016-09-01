#include "stdafx.h"
#include "EventManager.h"

EventManager g_EventManager;

MemoryPool Allocators::EventAllocator::s_MemoryPool("Event manager");

void EventManager::StartUp()
{
	Allocators::EventAllocator::Create();
}
void EventManager::ShutDown()
{
	m_EventListenersMap.clear();
	for (UINT i = 0; i < MAX_QUEUES; i++)
	{
		m_EventQueues[i].clear();
	}

	Allocators::EventAllocator::Destroy();
}
void EventManager::AddListener(EventListener &eventListener, ULONG eventType)
{
	// Check whether this event listener has not allready been added
	auto &eventListeners = m_EventListenersMap[eventType];
	for (EventListener &listener : eventListeners)
	{
		if (listener == eventListener)
		{
			EIGHT_WARNING("You are trying to add the same event listener twice");
			return;
		}
	}

	eventListeners.push_back(eventListener);

	EIGHT_LOG("Event listener was successfully registered");
}
void EventManager::RemoveListener(UINT eventListenerId, ULONG eventType)
{
	auto &eventListeners = m_EventListenersMap[eventType];
	for (auto eventListenerIt = eventListeners.begin();
		eventListenerIt != eventListeners.end(); eventListenerIt++)
	{
		EventListener &eventListener = *eventListenerIt;
		if (eventListener == eventListenerId)
		{
			eventListeners.erase(eventListenerIt);

			EIGHT_LOG("Event listener was successfully removed");
			return;
		}
	}
	EIGHT_WARNING("You are trying to remove event listener which was allready removed or it was not created");
}
void EventManager::Update()
{
	// Select active queue
	auto &activeQueue = m_EventQueues[m_ActiveQueue];
	if (++m_ActiveQueue >= MAX_QUEUES)
	{
		m_ActiveQueue = 0;
	}

	auto eventIT = activeQueue.begin();
	while (eventIT != activeQueue.end())
	{
		auto &eventData = *eventIT;

		// Send event to event listeners
		auto listeners = m_EventListenersMap[eventData->VGetEventDataType()];
		for (auto &listener : listeners)
		{
			listener(eventData);
		}

		// Remove event from queue
		auto erasableIT = eventIT++;
		activeQueue.erase(erasableIT);
	}
}