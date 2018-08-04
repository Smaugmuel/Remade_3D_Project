#ifndef SYSTEM_MANAGER_HPP
#define SYSTEM_MANAGER_HPP
#include "Singleton.hpp"
#include "System.hpp"
#include <map>

class SystemManager final : public Singleton<SystemManager>
{
public:

	/*
	Add a system which already exists
	*/
	template<typename SystemType> void AddExistingSystem(SystemType* system)
	{
		m_systems[SystemType::GetType()] = system;
	}

	/*
	Create and add a system which does not already exist
	*/
	template<typename SystemType, typename ... Args> void AddNewSystem(const Args& ... args)
	{
		m_system[SystemType::GetType()] = new SystemType(args...);
	}

	/*
	Retrieve a pointer to the system being requested if it exists
	Otherwise, retrieve a nullptr
	*/
	template<typename SystemType> SystemType* GetSystem()
	{
		if (m_systems.find(SystemType::GetType()) == m_systems.end())
		{
			return nullptr;
		}

		return static_cast<SystemType*>(m_systems.at(SystemType::GetType()));
	}

	/*
	Update only the chosen system
	*/
	template<typename SystemType> void UpdateOneSystem(float dt)
	{
		if (m_systems.find(SystemType::GetType()) == m_systems.end())
		{
			return;
		}

		m_systems.at(SystemType::GetType())->Update(dt);
	}

	/*
	Update all systems
	*/
	void UpdateAllSystems(float dt)
	{
		std::map<BaseSystem::SystemType, BaseSystem*>::iterator it;

		for (it = m_systems.begin(); it != m_systems.end(); ++it)
		{
			it->second->Update(dt);
		}
	}

private:
	friend class Singleton<SystemManager>;
	SystemManager()
	{
	}

	~SystemManager()
	{
	}


	std::map<BaseSystem::SystemType, BaseSystem*> m_systems;
};

#endif