#ifndef ENTITY_STORAGE_HPP
#define ENTITY_STORAGE_HPP
#include "Singleton.hpp"
#include <map>
#include <list>
#include "ComponentType.hpp"

typedef unsigned int EntityID;
typedef unsigned int ComponentsOfEntity;

class EntityStorage final : public Singleton<EntityStorage>
{
	friend class Singleton<EntityStorage>;
	EntityStorage();
	~EntityStorage();

public:
	void CreateEntity();
	void RemoveEntity(EntityID entity);

	void AttachComponent(EntityID entity, ComponentType componentType);
	void DetachComponent(EntityID entity, ComponentType componentType);

	bool DoesEntityExist(EntityID entity) const;

	unsigned int GetNrOfEntities() const;

	ComponentsOfEntity GetComponentsOfEntity(EntityID entity) const;

private:
	std::map<EntityID, ComponentsOfEntity> m_componentsOfEntity;
	
	//std::vector<EntityID> m_entities;

	std::list<EntityID> m_availableEntityIDs;
};

#endif