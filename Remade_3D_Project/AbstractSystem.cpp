#include "AbstractSystem.hpp"
#include "EntityStorage.hpp"
#include "EventDispatcher.hpp"

AbstractSystem::AbstractSystem()
{
	EventDispatcher::Get()->Subscribe(EventType::CREATED_ENTITY, this);
	EventDispatcher::Get()->Subscribe(EventType::REMOVED_ENTITY, this);
	EventDispatcher::Get()->Subscribe(EventType::ATTACHED_COMPONENT, this);
	EventDispatcher::Get()->Subscribe(EventType::DETACHED_COMPONENT, this);
}

AbstractSystem::~AbstractSystem()
{
	EventDispatcher::Get()->Unsubscribe(EventType::CREATED_ENTITY, this);
	EventDispatcher::Get()->Unsubscribe(EventType::REMOVED_ENTITY, this);
	EventDispatcher::Get()->Unsubscribe(EventType::ATTACHED_COMPONENT, this);
	EventDispatcher::Get()->Unsubscribe(EventType::DETACHED_COMPONENT, this);
}

void AbstractSystem::CreateEntity(EntityID entity)
{
	// Check if entity exists in system
	for (unsigned int i = 0; i < m_entitiesInSystem.size(); i++)
	{
		if (m_entitiesInSystem[i] == entity)
		{
			return;
		}
	}

	// Check if entity exists, but isn't in system
	for (unsigned int i = 0; i < m_entitiesNotInSystem.size(); i++)
	{
		if (m_entitiesNotInSystem[i] == entity)
		{
			return;
		}
	}

	// Add entity, but not to system yet
	m_entitiesNotInSystem.push_back(entity);
}

void AbstractSystem::RemoveEntity(EntityID entity)
{
	// Check if entity exists in system
	for (unsigned int i = 0; i < m_entitiesInSystem.size(); i++)
	{
		if (m_entitiesInSystem[i] == entity)
		{
			m_entitiesInSystem.erase(m_entitiesInSystem.begin() + i);
			return;
		}
	}

	// Check if entity exists, but isn't in system
	for (unsigned int i = 0; i < m_entitiesNotInSystem.size(); i++)
	{
		if (m_entitiesNotInSystem[i] == entity)
		{
			m_entitiesNotInSystem.erase(m_entitiesNotInSystem.begin() + i);
			return;
		}
	}
}

void AbstractSystem::AddEntityToSystem(EntityID entity)
{
	std::vector<EntityID>::iterator it = std::find(m_entitiesNotInSystem.begin(), m_entitiesNotInSystem.end(), entity);

	if (it == m_entitiesNotInSystem.end())
	{
		// Entity is not in the "not in system" list
		// In other words, it is in the system
		return;
	}

	if (HasCorrectComponents(entity))
	{
		m_entitiesInSystem.push_back(entity);
		m_entitiesNotInSystem.erase(it);
	}
}

void AbstractSystem::RemoveEntityFromSystem(EntityID entity)
{
	std::vector<EntityID>::iterator it = std::find(m_entitiesInSystem.begin(), m_entitiesInSystem.end(), entity);

	if (it == m_entitiesInSystem.end())
	{
		// Entity is not in the system
		// In other words, it is in the "not in system" list
		return;
	}

	if (!HasCorrectComponents(entity))
	{
		m_entitiesNotInSystem.push_back(entity);
		m_entitiesInSystem.erase(it);
	}
}

void AbstractSystem::ReceiveEvent(const Event & e)
{
	switch (e.type)
	{
	case EventType::CREATED_ENTITY:
		CreateEntity(*static_cast<EntityID*>(e.value));
		break;
	case EventType::REMOVED_ENTITY:
		RemoveEntity(*static_cast<EntityID*>(e.value));
		break;
	case EventType::ATTACHED_COMPONENT:
		AddEntityToSystem(*static_cast<EntityID*>(e.value));
		break;
	case EventType::DETACHED_COMPONENT:
		RemoveEntityFromSystem(*static_cast<EntityID*>(e.value));
	default:
		break;
	}
}