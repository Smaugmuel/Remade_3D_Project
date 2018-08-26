#ifndef CAMERA_V3_HPP
#define CAMERA_V3_HPP
#include "../Math/Matrix.hpp"

struct CameraV3
{
	CameraV3();
	CameraV3(const CameraV3& other);
	~CameraV3();

	CameraV3& operator=(const CameraV3& right);

	void UpdateViewMatrix();
	void UpdateProjectionMatrix();

	Vector3f position;
	Vector3f target;
	Vector3f up;
	float fov;
	float aspectRatio;
	float nearPlane;
	float farPlane;

	Matrix viewMatrix;
	Matrix projectionMatrix;
};

#endif