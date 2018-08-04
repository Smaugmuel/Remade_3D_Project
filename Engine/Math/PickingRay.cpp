#include "PickingRay.hpp"

// For getting mouse position
#include "../Input/Input.hpp"
#include "../FrameWork/WindowSettings.hpp"

// For getting the corners of the camera's frustum
#include "../Camera/PlayerCameraManager.hpp"
#include "../Camera/Camera.hpp"
#include "Frustum.hpp"

Ray HF::CreatePickingRay()
{
	Camera* cam = PlayerCameraManager::Get()->GetCurrentCamera();
	Vector3f corners[8];
	Ray ray;

	// The 8 corners of frustum in world coordinates
	FrustumCorners(cam->GetViewMatrix(), cam->GetProjectionMatrix(), corners);

	// Mouse coordinates on screen, in range [(0, 0), (WNDW, WNDH)]
	Vector2i mousePos = Input::Get()->MousePosition();

	// Mouse coordinates in percent, in range [(0, 0), (1, 1)]
	Vector2f mousePercentage = mousePos;
	mousePercentage.x /= WNDW;
	mousePercentage.y /= WNDH;
	mousePercentage.y = 1 - mousePercentage.y;

	Vector3f mouseDirectionOnFarPlaneX = (corners[5] - corners[1]) * mousePercentage.x;
	Vector3f mouseDirectionOnFarPlaneY = (corners[3] - corners[1]) * mousePercentage.y;
	Vector3f mouseDirectionOnNearPlaneX = (corners[4] - corners[0]) * mousePercentage.x;
	Vector3f mouseDirectionOnNearPlaneY = (corners[2] - corners[0]) * mousePercentage.y;

	Vector3f mousePositionOnFarPlane = (corners[1] + mouseDirectionOnFarPlaneX + mouseDirectionOnFarPlaneY);
	Vector3f mousePositionOnNearPlane = (corners[0] + mouseDirectionOnNearPlaneX + mouseDirectionOnNearPlaneY);

	ray.direction = (mousePositionOnFarPlane - mousePositionOnNearPlane).normalized();
	ray.origin = mousePositionOnNearPlane;

	return ray;
}
