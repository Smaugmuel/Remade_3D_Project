#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP
#include "Singleton.hpp"
#include <list>
#include "ComponentWrapper.hpp"

typedef unsigned int EntityID;

class EntityManager : public Singleton<EntityManager>
{
public:

	EntityID CreateEntity();
	void DestroyEntity(EntityID entity);

	template<typename Type, typename ... Args> ComponentWrapper<Type> AttachComponent(EntityID entity, const Args& ... args)
	{
		ComponentWrapper<Type> wrapper(entity);
	}

private:
	friend class Singleton<EntityManager>;
	EntityManager();
	~EntityManager();


	std::list<EntityID> m_availableEntityIDs;
};

#endif