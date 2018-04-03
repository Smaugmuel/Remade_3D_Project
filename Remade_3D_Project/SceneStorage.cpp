#include "SceneStorage.hpp"
#include "Scene.hpp"

#include <Windows.h>

SceneStorage* Singleton<SceneStorage>::s_instance = nullptr;

SceneStorage::SceneStorage()
{
}

SceneStorage::~SceneStorage()
{
}

void SceneStorage::UpdateExistingSceneNamesFromFolder()
{
	void* handle;
	_WIN32_FIND_DATAW fileData;
	const wchar_t* wcfileName = L"../Scenes/*.txt";

	handle = FindFirstFileW(wcfileName, &fileData);

	// Check if at least one file was found
	if (handle != INVALID_HANDLE_VALUE)
	{
		m_existingSceneNames.clear();

		// Remove ".txt" before adding to vector
		std::wstring wFileName = fileData.cFileName;
		wFileName.erase(wFileName.size() - 4, 4);
		m_existingSceneNames.push_back(wFileName);

		// Check each existing file after the first one
		while (FindNextFileW(handle, &fileData))
		{
			// Remove ".txt" before adding to vector
			wFileName = fileData.cFileName;
			wFileName.erase(wFileName.size() - 4, 4);
			m_existingSceneNames.push_back(wFileName);
		}

	}

	// Close handle
	FindClose(handle);
}

const std::vector<std::wstring>& SceneStorage::GetExistingSceneNames() const
{
	return m_existingSceneNames;
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
