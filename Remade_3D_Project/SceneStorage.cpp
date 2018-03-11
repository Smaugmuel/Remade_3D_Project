#include "SceneStorage.hpp"
#include "Scene.hpp"

SceneStorage* Singleton<SceneStorage>::s_instance = nullptr;

SceneStorage::SceneStorage()
{
}

SceneStorage::~SceneStorage()
{
}

bool SceneStorage::LoadScene(const std::string & name)
{
	if (HasScene(name))
	{
		return true;
	}

	m_scenes[name] = std::make_unique<Scene>();
	
	return m_scenes.at(name).get()->LoadFromFile(name);
}

Scene * SceneStorage::GetScene(const std::string & name)
{
	return m_scenes.at(name).get();
}

bool SceneStorage::HasScene(const std::string & name) const
{
	return m_scenes.find(name) != m_scenes.end();
}
