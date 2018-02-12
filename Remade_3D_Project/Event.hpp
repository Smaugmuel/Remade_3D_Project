#ifndef EVENT_HPP
#define EVENT_HPP

enum EventType
{
	// Types from previous program, most will not be needed

	CREATED_ENTITY,
	DELETED_ENTITY,

	ATTACHED_GRAVITY_TO_ENTITY,
	DETACHED_GRAVITY_FROM_ENTITY,

	ATTACHED_COLLISION_WITH_ENTITIES_TO_ENTITY,
	DETACHED_COLLISION_WITH_ENTITIES_FROM_ENTITY,

	KEY_PRESSED,
	KEY_DOWN,

	MOVED_VIEW,
	ZOOMED_VIEW,

	BLOCK_DESTROYED,
	BLOCK_CREATED,
	TURRET_DESTROYED,
	TURRET_CREATED,

	PLAYSTATE_LOADED,

	GENERATOR_BELOW_ZERO
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