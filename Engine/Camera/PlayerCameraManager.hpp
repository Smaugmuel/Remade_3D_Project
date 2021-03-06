#ifndef PLAYER_CAMERA_MANAGER_HPP
#define PLAYER_CAMERA_MANAGER_HPP
//#include "../Misc/Singleton.hpp"
#include <vector>
#include <memory>
#include "../Math/Vector2.hpp"

class Camera;

class PlayerCameraManager final/* : public Singleton<PlayerCameraManager>*/
{
	//friend class Singleton <PlayerCameraManager>;

public:
	PlayerCameraManager();
	~PlayerCameraManager();

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