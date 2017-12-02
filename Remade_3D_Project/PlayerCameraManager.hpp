#ifndef PLAYER_CAMERA_MANAGER_HPP
#define PLAYER_CAMERA_MANAGER_HPP
#include "Singleton.hpp"
#include <vector>
#include <memory>
#include "Vector2.hpp"

class Camera;

class PlayerCameraManager : public Singleton<PlayerCameraManager>
{
	friend class Singleton <PlayerCameraManager>;

	PlayerCameraManager();
	~PlayerCameraManager();

public:
	bool Initialize();

	void Update();

	Camera* CreateCamera();

	void ChangeTo(unsigned int i);
	void ChangeUp();
	void ChangeDown();
	
	Camera* GetCurrentCamera();
	Camera* GetCamera(unsigned int i);

	unsigned int GetNrOfCameras() const;

private:
	std::vector<std::unique_ptr<Camera>> m_cameras;
	int m_currentCamera;
	Vector2i m_dimensions;
};

#endif