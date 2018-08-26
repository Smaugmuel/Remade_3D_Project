#ifndef CAMERA_V2_HPP
#define CAMERA_V2_HPP
#include "../Math/Vector2.hpp"
#include "../Math/Vector3.hpp"

struct CameraV2
{
	Vector3f position;
	Vector3f target;
	Vector3f up;

	Vector2f dimensions;
	float fov;
	float nearPlane;
	float farPlane;

	int cameraID;
	int viewMatrixID;
	int projectionMatrixID;
};

#endif