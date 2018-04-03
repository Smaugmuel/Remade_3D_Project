#include "EntityComponentManager.hpp"
#include "PhysicSystem.hpp"
#include "ComponentStorage.hpp"
#include <time.h>

EntityComponentManager* Singleton<EntityComponentManager>::s_instance = nullptr;

EntityComponentManager::EntityComponentManager()
{
	m_systems.push_back(new PhysicSystem);

	EntityID entity = ComponentStorage::Get()->CreateEntity();
	ComponentStorage::Get()->AttachComponent<TransformComponent>(entity);
	ComponentStorage::Get()->AttachComponent<PhysicComponent>(entity);

	/*// Perform random actions to test that system doesn't crash or behaves incorrectly
	srand(time(NULL));
	unsigned int highestEntity = 1;
	ComponentStorage* componentStorage = ComponentStorage::Get();

	for (unsigned int i = 0; i < 10000; i++)
	{
		switch (rand() % 7)
		{
		case 0:
			highestEntity = std::max(componentStorage->CreateEntity(), highestEntity);
			break;
		case 1:
			componentStorage->DestroyEntity(rand() % highestEntity);
			break;
		case 2:
			componentStorage->AttachComponent<TransformComponent>(rand() % highestEntity);
			break;
		case 3:
			componentStorage->AttachComponent<PhysicComponent>(rand() % highestEntity);
			break;
		case 4:
			componentStorage->DetachComponent<TransformComponent>(rand() % highestEntity);
			break;
		case 5:
			componentStorage->DetachComponent<PhysicComponent>(rand() % highestEntity);
			break;
		case 6:
		{
			TransformComponent* transform = componentStorage->GetComponent<TransformComponent>(rand() % highestEntity);
			PhysicComponent* physic = componentStorage->GetComponent<PhysicComponent>(rand() % highestEntity);
			if (!transform || !physic)
			{
				break;
			}

			transform->position = Vector3f(0, 0, 0);
			transform->rotation = Vector3f(0, 0, 0);

			physic->velocity = Vector3f(100, 0, 0);
			physic->acceleration = Vector3f(-100, 0, 0);
			physic->angularVelocity = Vector3f(0, 200, 0);
			physic->angularAcceleration = Vector3f(0, -200, 0);
		}
		default:
			break;
		}
	}*/
}

EntityComponentManager::~EntityComponentManager()
{
	for (unsigned int i = 0; i < m_systems.size(); i++)
	{
		delete m_systems[i];
	}
}

void EntityComponentManager::CreateEntity()
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
		// An old entity is reused
		entity = m_availableEntities.front();
		m_availableEntities.pop_front();
	}

	// Add entity
	m_entities.push_back(entity);
	EventDispatcher::Get()->Emit(Event(EventType::CREATED_ENTITY, &entity));
}

void EntityComponentManager::DestroyEntity(EntityID entity)
{

}

void EntityComponentManager::Update(float dt)
{
	for (unsigned int i = 0; i < m_systems.size(); i++)
	{
		m_systems[i]->Update(dt);
	}
}