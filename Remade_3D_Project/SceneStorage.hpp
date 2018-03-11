#ifndef SCENE_STORAGE_HPP
#define SCENE_STORAGE_HPP
#include "Singleton.hpp"

#include <map>
#include <memory>
#include <string>

class Scene;

class SceneStorage final : public Singleton<SceneStorage>
{
	friend class Singleton<SceneStorage>;

	typedef std::map<std::string, std::unique_ptr<Scene>> SceneMap;

	SceneStorage();
	~SceneStorage();

public:
	bool LoadScene(const std::string& name);

	Scene * GetScene(const std::string& name);

	bool HasScene(const std::string& name) const;


private:
	SceneMap m_scenes;
};

#endif