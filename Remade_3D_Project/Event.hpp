#ifndef EVENT_HPP
#define EVENT_HPP

enum class EventType
{
	POP_GAMESTATE
};

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