#include "PhysicSystem.hpp"
#include "EntityStorage.hpp"
#include "ComponentStorage.hpp"

PhysicSystem::PhysicSystem()
{
}

PhysicSystem::~PhysicSystem()
{
}

void PhysicSystem::Update(float dt)
{
	ComponentStorage* componentStorage = ComponentStorage::Get();

	// Update all components of entities in this system
	for (unsigned int i = 0; i < m_entitiesInSystem.size(); i++)
	{
		EntityID entity = m_entitiesInSystem[i];
		PhysicComponent* physic = componentStorage->GetComponent<PhysicComponent>(entity);
		TransformComponent* transform = componentStorage->GetComponent<TransformComponent>(entity);

		transform->position += physic->velocity * dt;
		physic->velocity += physic->acceleration * dt;

		transform->rotation += physic->angularVelocity * dt;
		physic->angularVelocity += physic->angularAcceleration * dt;
	}
}

bool PhysicSystem::HasCorrectComponents(EntityID entity) const
{
	ComponentStorage* componentStorage = ComponentStorage::Get();
	
	// Check if entity has transformation and physic
	return componentStorage->GetComponent<TransformComponent>(entity)
		&& componentStorage->GetComponent<PhysicComponent>(entity);
}