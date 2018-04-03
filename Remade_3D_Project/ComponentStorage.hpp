#ifndef COMPONENT_STORAGE_HPP
#define COMPONENT_STORAGE_HPP
#include "Singleton.hpp"
#include <map>
#include <memory>
#include <typeindex>
#include <list>
#include <algorithm>
#include "Components.hpp"
#include "EventDispatcher.hpp"

typedef unsigned int EntityID;

class ComponentStorage : public Singleton<ComponentStorage>
{
	friend class Singleton<ComponentStorage>;
	ComponentStorage()
	{
	}
	~ComponentStorage()
	{
	}

public:

	/**
	Creates an entity with a unique ID
	Will reuse old IDs if an entity was destroyed
	*/
	/*EntityID CreateEntity()
	{
		static EntityID nextEntityID = 0;

		EntityID entity;

		if (m_availableEntities.empty())
		{
			// No old IDs exist, create a new
			entity = nextEntityID++;
		}
		else
		{
			// An entity is reused
			entity = m_availableEntities.front();
			m_availableEntities.pop_front();
		}

		// Add entity
		m_entities.push_back(entity);
		EventDispatcher::Get()->Emit(Event(EventType::CREATED_ENTITY, &entity));

		return entity;
	}*/

	/**
	Destroys an entity and all components of it
	Its ID is made available when creating the next entity
	*/
	void DestroyEntity(EntityID entity)
	{
		std::map<std::type_index, std::map<EntityID, std::unique_ptr<Component>>>::iterator it;
		std::map<EntityID, std::unique_ptr<Component>>::iterator it2;
		bool wasDestroyed = false;

		// Iterate through each component type
		for (it = m_components.begin(); it != m_components.end(); ++it)
		{
			// Check if this entity has this component
			it2 = it->second.find(entity);
			
			if (it2 == it->second.end())
			{
				// This entity does not have this component
				continue;
			}

			wasDestroyed = true;

			// Remove this entity and this component
			it->second.erase(it2);
		}

		if (wasDestroyed)
		{
			EventDispatcher::Get()->Emit(Event(EventType::REMOVED_ENTITY, &entity));

			// Remove entity
			m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), entity));

			// Make the ID available again
			m_availableEntities.push_front(entity);
		}
	}

	/**
	Creates and attaches a component to an entity
	*/
	template<typename Type, typename ... Args> void AttachComponent(EntityID entity, const Args& ... args)
	{
		// Check if entity exists
		if (std::find(m_entities.begin(), m_entities.end(), entity) == m_entities.end())
		{
			return;
		}

		m_components[typeid(Type)][entity] = std::make_unique<Type>(args...);

		EventDispatcher::Get()->Emit(Event(EventType::ATTACHED_COMPONENT, &entity));
	}

	/**
	Detaches and removes a component from an entity
	*/
	template<typename Type> void DetachComponent(EntityID entity)
	{
		std::map<std::type_index, std::map<EntityID, std::unique_ptr<Component>>>::iterator it;
		std::map<EntityID, std::unique_ptr<Component>>::iterator it2;

		// Check if entity exists
		if (std::find(m_entities.begin(), m_entities.end(), entity) == m_entities.end())
		{
			return;
		}

		// Check if this component type exists
		it = m_components.find(typeid(Type));	
		if (it == m_components.end())
		{
			return;
		}

		// Check if this entity has this component
		it2 = it->second.find(entity);
		if (it2 == it->second.end())
		{
			return;
		}
		
		// Remove entity from this component type
		m_components.at(typeid(Type)).erase(entity);

		EventDispatcher::Get()->Emit(Event(EventType::DETACHED_COMPONENT, &entity));
	}

	/**
	Retrieves all components of a given type
	They will need to be cast to their component type outside before used
	*/
	template<typename Type> std::map<EntityID, std::unique_ptr<Component>>& GetAllComponentsOfType()
	{
		return m_components.at(typeid(Type));
	}

	/**
	Retrieves one component of a given type and entity
	Returns nullptr if failed to find this type or entity
	This is automatically cast to its type
	*/
	template<typename Type> Type* GetComponent(EntityID entity)
	{
		if (m_components.find(typeid(Type)) == m_components.end())
		{
			return nullptr;
		}
		if (m_components.at(typeid(Type)).find(entity) == m_components.at(typeid(Type)).end())
		{
			return nullptr;
		}

		return static_cast<Type*>(m_components.at(typeid(Type)).at(entity).get());
	}

private:
	std::map<std::type_index, std::map<EntityID, std::unique_ptr<Component>>> m_components;

	/*std::vector<EntityID> m_entities;
	std::list<EntityID> m_availableEntities;*/
};

ComponentStorage* Singleton<ComponentStorage>::s_instance = nullptr;

#endif