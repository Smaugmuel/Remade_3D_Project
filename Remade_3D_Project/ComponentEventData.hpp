#ifndef COMPONENT_EVENT_DATA_HPP
#define COMPONENT_EVENT_DATA_HPP

typedef unsigned int EntityID;
typedef unsigned int ComponentID;

struct ComponentEventData
{
	EntityID entity;
	ComponentID component;
};

#endif