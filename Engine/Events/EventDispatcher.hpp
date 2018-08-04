#ifndef EVENT_DISPATCHER_HPP
#define EVENT_DISPATCHER_HPP
#include "../Misc/Singleton.hpp"
#include "Event.hpp"
#include <vector>
#include <map>

/* Notes and instructions */
// =======================================================================================================================
// | This event dispatcher should never be declared manually, since it's a singleton                                     |
// | Preferably, include and use it only in .cpp-files                                                                   |
// -----------------------------------------------------------------------------------------------------------------------
// | When creating an event, the event type needs to be set and the value reference "needs" to be cast to a void pointer |
// | When emitting an event, create and send a new event in one go                                                       |
// -----------------------------------------------------------------------------------------------------------------------
// | I.e. EventDispatcher::Get()->Emit(Event(EventType::CREATED_ENTITY, static_cast<void*>(&playerPos)));                |
// =========================================================================================================================================
// | The class receiving events needs to inherit from EventReceiver and override void ReceiveEvent(const Event& e)                         |
// | In ReceiveEvent(const Event& e), the event type needs to be checked, preferably with a switch statement                               |
// | If the type matches with one of the cases, the value of the event needs to be statically cast to that data type before it can be used |
// -----------------------------------------------------------------------------------------------------------------------------------------
// | I.e.                                                                                                                                  |
// | void ReceiveEvent(const Event& e)                                                                                                     |
// | {                                                                                                                                     |
// |     switch(e.type)                                                                                                                    |
// |     {                                                                                                                                 |
// |     case CREATED_ENTITY:                                                                                                              |
// |         Vector3* pos = static_cast<Vector3*>(e.value);                                                                                |
// |         // Use pos here                                                                                                               |
// |         break;                                                                                                                        |
// |     }                                                                                                                                 |
// | }                                                                                                                                     |
// =========================================================================================================================================
// | Receivers can only receive events of a type they're subscribed to                                                     |
// | Nothing "bad" will happen though, if a receiver is subscribed to an event without having a case for it, or vice versa |
// | One receiver can be subscribed to multiple event types, but will only become a subscriber once per type               |
// -------------------------------------------------------------------------------------------------------------------------
// | A receiver can either subscribe itself, or have another class do it. The same applies with unsubscribing              |
// -------------------------------------------------------------------------------------------------------------------------
// | I.e. EventDispatcher::Get()->Subscribe(EventType::CREATED_ENTITY, &entityManager);                                    |
// =========================================================================================================================
// | It is recommended (but not always necessary) for a class to unsubscribe itself from all events when it's destroyed |
// | Otherwise the dispatcher might attempt to call ReceiveEvent() on a destroyed object                                |
// ======================================================================================================================

class EventReceiver;

class EventDispatcher final : public Singleton<EventDispatcher>
{
	friend class Singleton<EventDispatcher>;

	// Define data types for easier use
	typedef std::vector<EventReceiver*> EventReceivers;
	typedef std::map<EventType, EventReceivers> ReceiverMap;

	EventDispatcher();
	~EventDispatcher();

public:
	void Emit(const Event& e);
	void Subscribe(const EventType& type, EventReceiver* subscriber);
	void Unsubscribe(const EventType& type, EventReceiver* subscriber);

private:
	bool EventTypeExists(const EventType& type) const;
	bool Subscribed(const EventType& type, EventReceiver* subscriber);

	ReceiverMap m_subscribers;
};

#endif