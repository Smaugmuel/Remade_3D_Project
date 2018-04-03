#include "EntityStorage.hpp"
#include "EventDispatcher.hpp"

EntityStorage* Singleton<EntityStorage>::s_instance = nullptr;

EntityStorage::EntityStorage()
{
	// Add possibility to add one entity (list should never have 0 IDs)
	m_availableEntityIDs.push_back(0);
}

EntityStorage::~EntityStorage()
{
}

void EntityStorage::CreateEntity()
{
	static unsigned int nextID = 0;

	// Take the first available ID
	EntityID id = m_availableEntityIDs.front();

	// If only one ID remains, add one more
	// This ensures there will be at least one after it being popped below
	if (m_availableEntityIDs.size() == 1)
	{
		// Add new unique ID to list
		m_availableEntityIDs.push_back(++nextID);
	}

	// Set no components
	m_componentsOfEntity[m_availableEntityIDs.front()] = 0;

	// Remove ID from list
	m_availableEntityIDs.pop_front();

	EventDispatcher::Get()->Emit(Event(EventType::CREATED_ENTITY, &id));
}

void EntityStorage::RemoveEntity(EntityID entity)
{
	std::map<EntityID, ComponentsOfEntity>::iterator it = m_componentsOfEntity.find(entity);

	// Check if this entity exists
	if (it == m_componentsOfEntity.end())
	{
		// Entity does not exist
		return;
	}

	// Make this entity ID available again
	// Push to front to reuse ID instead of using a new one (i.e. next entity created uses 1 instead of 4)
	m_availableEntityIDs.push_front(entity);

	// Remove entity
	m_componentsOfEntity.erase(it);
	
	EventDispatcher::Get()->Emit(Event(EventType::REMOVED_ENTITY, &entity));
}

void EntityStorage::AttachComponent(EntityID entity, ComponentType componentType)
{
	if (!DoesEntityExist(entity))
	{
		return;
	}

	ComponentsOfEntity& comps = m_componentsOfEntity.at(entity);
	
	// Check if entity already has that component (if bit at this significance is 1)
	if (comps & (1 << componentType))
	{
		return;
	}

	// Set entity to have this component (set bit at this significance to 1)
	comps |= (1 << componentType);

	EventDispatcher::Get()->Emit(Event(EventType::ATTACHED_COMPONENT, &entity));
}

void EntityStorage::DetachComponent(EntityID entity, ComponentType componentType)
{
	if (!DoesEntityExist(entity))
	{
		return;
	}

	ComponentsOfEntity& comps = m_componentsOfEntity.at(entity);

	// Check if entity already has that component (if bit at this significance is 1)
	if (comps & (1 << componentType))
	{
		// Set entity to not have this component (set bit at this significance to 0)
		comps &= ~(1 << componentType);

		EventDispatcher::Get()->Emit(Event(EventType::DETACHED_COMPONENT, &entity));
	}
}

bool EntityStorage::DoesEntityExist(EntityID entity) const
{
	return !(m_componentsOfEntity.find(entity) == m_componentsOfEntity.end());
}

unsigned int EntityStorage::GetNrOfEntities() const
{
	return m_componentsOfEntity.size();
}

ComponentsOfEntity EntityStorage::GetComponentsOfEntity(EntityID entity) const
{
	if (!DoesEntityExist(entity))
	{
		return 0;
	}

	return m_componentsOfEntity.at(entity);
}