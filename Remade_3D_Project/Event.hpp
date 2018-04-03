#ifndef EVENT_HPP
#define EVENT_HPP
#include "EventTypes.hpp"

struct Event
{
	Event(const EventType& _type, void* _value = nullptr) :
		type(_type),
		value(_value)
	{
	}
	EventType type;
	void* value;
};

#endif