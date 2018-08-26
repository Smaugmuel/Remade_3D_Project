#ifndef CAMERA_MANAGER_HPP
#define CAMERA_MANAGER_HPP
#include "../FrameWork/ReusableStorage.hpp"
#include "CameraV2.hpp"

class MatrixManager;

class CameraManager
{
public:
	CameraManager();
	~CameraManager();

	bool Initialize(MatrixManager* matrixManager);

	int CreateCamera(Vector3f position, Vector3f target, Vector3f up, float fov, Vector2f dimensions, float nearPlane, float farPlane);

	CameraV2* GetCamera(int id);

private:
	ReusableStorage<CameraV2> m_cameras;
	MatrixManager* m_matrixManager;
};

#endif