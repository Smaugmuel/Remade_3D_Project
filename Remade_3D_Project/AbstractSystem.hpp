#ifndef ABSTRACT_SYSTEM_HPP
#define ABSTRACT_SYSTEM_HPP
#include "EventReceiver.hpp"
#include <vector>

typedef unsigned int EntityID;

class AbstractSystem : public EventReceiver
{
public:
	AbstractSystem();
	virtual ~AbstractSystem();
	
	virtual void Update(float dt) = 0;

protected:
	std::vector<EntityID> m_entitiesInSystem;
	std::vector<EntityID> m_entitiesNotInSystem;

private:
	void CreateEntity(EntityID entity);
	void RemoveEntity(EntityID entity);
	void AddEntityToSystem(EntityID entity);
	void RemoveEntityFromSystem(EntityID entity);

	virtual bool HasCorrectComponents(EntityID entity) const = 0;

	void ReceiveEvent(const Event& e) override;
};

#endif