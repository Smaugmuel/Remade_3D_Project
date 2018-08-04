#include "EventDispatcher.hpp"
#include "EventReceiver.hpp"
#include <algorithm>

EventDispatcher* Singleton<EventDispatcher>::s_instance = nullptr;

EventDispatcher::EventDispatcher()
{
}

EventDispatcher::~EventDispatcher()
{
}

void EventDispatcher::Emit(const Event & e)
{
	// Don't emit if no one ever subscribed to event type
	if (!EventTypeExists(e.type))
		return;

	// Emit to each subscriber of this event type
	EventReceivers& subs = m_subscribers[e.type];
	for (unsigned int i = 0; i < subs.size(); i++)
	{
		subs[i]->ReceiveEvent(e);
	}
}

void EventDispatcher::Subscribe(const EventType & type, EventReceiver * subscriber)
{
	// Prevent receivers from having multiple subscriptions to one event type
	if (Subscribed(type, subscriber))
		return;

	// Add subscriber to the list
	EventReceivers& receivers = m_subscribers[type];
	receivers.push_back(subscriber);
}

void EventDispatcher::Unsubscribe(const EventType & type, EventReceiver * subscriber)
{
	// The code below this would work, but is unnecessary to run if no one is subscribed
	if (!EventTypeExists(type))
		return;

	// This is known as the "erase remove idiom"
	// remove() searches an iterable array for an object, places each of them last in the array, and returns the iterator to the first of them
	// erase() then removes from the first of those objects until the end of the array
	EventReceivers& receivers = m_subscribers[type];
	receivers.erase(std::remove(receivers.begin(), receivers.end(), subscriber), receivers.end());
}

bool EventDispatcher::EventTypeExists(const EventType & type) const
{
	// By using find() and end() instead of i.e. "map[type].size() > 0", the map key is not automatically created
	return m_subscribers.find(type) != m_subscribers.end();
}

bool EventDispatcher::Subscribed(const EventType & type, EventReceiver * subscriber)
{
	// The code below this would work, but is unnecessary to run if no one is subscribed
	if (!EventTypeExists(type))
		return false;

	// Check if the subscriber exists in the vector
	EventReceivers& receivers = m_subscribers[type];
	return std::find(receivers.begin(), receivers.end(), subscriber) != receivers.end();
}
