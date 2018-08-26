#include "CameraManager.hpp"
#include "../FrameWork/MatrixManager.hpp"

CameraManager::CameraManager()
{
}

CameraManager::~CameraManager()
{
}

bool CameraManager::Initialize(MatrixManager * matrixManager)
{
	m_matrixManager = matrixManager;
	return true;
}

int CameraManager::CreateCamera(Vector3f position, Vector3f target, Vector3f up, float fov, Vector2f dimensions, float nearPlane, float farPlane)
{
	int id = m_cameras.CreateElementAndGetIndex();
	CameraV2* camera = m_cameras.GetElement(id);

	camera->position = position;
	camera->target = target;
	camera->up = up;
	camera->fov = fov;
	camera->dimensions = dimensions;
	camera->nearPlane = nearPlane;
	camera->farPlane = farPlane;

	camera->cameraID = id;
	camera->viewMatrixID = m_matrixManager->CreateViewMatrix();
	camera->projectionMatrixID = m_matrixManager->CreateProjectionMatrix();

	return camera->cameraID;
}

CameraV2* CameraManager::GetCamera(int id)
{
	return m_cameras.GetElement(id);
}
