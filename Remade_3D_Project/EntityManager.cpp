#include "EntityManager.hpp"
#include "EventDispatcher.hpp"

EntityManager* Singleton<EntityManager>::s_instance = nullptr;

EntityManager::EntityManager()
{
	// Add possibility to add one entity (list should never have 0 IDs)
	m_availableEntityIDs.push_back(0);
}

EntityManager::~EntityManager()
{
}

EntityID EntityManager::CreateEntity()
{
	static EntityID nextID = 0;

	// Take the first available ID
	EntityID id = m_availableEntityIDs.front();

	// If only one ID remains, add one more
	// This ensures there will be at least one after it being popped below
	if (m_availableEntityIDs.size() == 1)
	{
		// Add new unique ID to list
		m_availableEntityIDs.push_back(++nextID);
	}

	// Remove ID from list
	m_availableEntityIDs.pop_front();

	EventDispatcher::Get()->Emit(Event(EventType::CREATED_ENTITY, &id));

	return id;
}

void EntityManager::DestroyEntity(EntityID entity)
{
}