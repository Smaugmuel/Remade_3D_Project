#ifndef EVENT_RECEIVER_HPP
#define EVENT_RECEIVER_HPP
#include "Event.hpp"

// Each inheriting class needs to check which event type was sent, and then static_cast the value to a pointer of the corresponding value type

class EventReceiver
{
public:
	EventReceiver() {}
	virtual ~EventReceiver() {}

	virtual void ReceiveEvent(const Event& e) = 0;
};

#endif