#include "EventsManager.h"

static TMap<E_EventType, TArray<Listener>> eventTable;

EventsManager::EventsManager(){}

void EventsManager::AddListener(E_EventType eventType, const Listener& listener)
{
	if (eventTable.Contains(eventType))
	{
		eventTable[eventType].Add(listener);
	}
	else
	{
		eventTable.Add(eventType, {listener});
	}
}

void EventsManager::RemoveListener(E_EventType eventType, const Listener& listener)
{
	if (!eventTable.Contains(eventType))
	{
		return;
	}

	TArray<Listener>& listeners = eventTable[eventType];
	listeners.Remove(listener);
} 

void EventsManager::SendEvent(E_EventType eventType, const int32 intParam)
{
	if (!eventTable.Contains(eventType))
	{
		return;
	}
	
	TArray<Listener>& listeners = eventTable[eventType];

	for (const Listener& listener : listeners)
	{
		if (listener.m_OnCallback.IsBound())
		{
			listener.m_OnCallback.Execute(eventType, intParam);
		}
	}
}