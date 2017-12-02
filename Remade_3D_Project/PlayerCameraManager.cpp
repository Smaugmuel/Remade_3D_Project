#include "PlayerCameraManager.hpp"
#include "Camera.hpp"

PlayerCameraManager* Singleton<PlayerCameraManager>::s_instance = nullptr;

PlayerCameraManager::PlayerCameraManager() :
	m_currentCamera(-1)
{
}

PlayerCameraManager::~PlayerCameraManager()
{
}

bool PlayerCameraManager::Initialize()
{
	m_currentCamera = -1;
	return true;
}

void PlayerCameraManager::Update()
{
	for (unsigned int i = 0; i < m_cameras.size(); i++)
	{
		m_cameras[i]->Update();
	}
}

Camera * PlayerCameraManager::CreateCamera()
{
	m_cameras.push_back(std::make_unique<Camera>());
	m_cameras.back()->Initialize();

	if (m_cameras.size() == 1)
	{
		// Only one camera exists, therefore make it the current one
		m_currentCamera = 0;
	}
	else
	{
		// Create as a copy of the current one
		*m_cameras.back() = *GetCurrentCamera();
	}

	return m_cameras.back().get();
}

void PlayerCameraManager::ChangeTo(unsigned int i)
{
	if (i >= 0 && i < m_cameras.size())
	{
		m_currentCamera = i;
	}
}

void PlayerCameraManager::ChangeUp()
{
	if (m_cameras.size() > 0)
	{
		m_currentCamera = (m_currentCamera + 1) % m_cameras.size();
	}
}

void PlayerCameraManager::ChangeDown()
{
	if (m_cameras.size() > 0)
	{
		// Couldn't go below 0 when size was 3
		//m_currentCamera = (m_currentCamera - 1) % m_cameras.size();
		m_currentCamera = (m_currentCamera == 0 ? m_cameras.size() - 1 : m_currentCamera - 1);
	}
}

Camera * PlayerCameraManager::GetCurrentCamera()
{
	return (m_cameras.size() > 0 ? m_cameras[m_currentCamera].get() : nullptr);
}

Camera * PlayerCameraManager::GetCamera(unsigned int i)
{
	return (i >= 0 && i < m_cameras.size() ? m_cameras[m_currentCamera].get() : nullptr);
}

unsigned int PlayerCameraManager::GetNrOfCameras() const
{
	return m_cameras.size();
}