#ifndef CAMERA_V3_HPP
#define CAMERA_V3_HPP
#include "../Math/Math.hpp"

struct CameraV3
{
	CameraV3();
	CameraV3(const CameraV3& other);
	~CameraV3();

	CameraV3& operator=(const CameraV3& right);

	void UpdateViewMatrix();
	void UpdateProjectionMatrix();

	void MoveForward(float amount);
	void MoveRight(float amount);
	void MoveUp(float amount);
	void RotateUp(float angle);
	void RotateRight(float angle);

	Vector3f TargetDirection() const;
	Vector3f Right() const;
	Vector3f Forward() const;

	Vector3f position;
	Vector3f target;
	Vector3f up;
	float fov;
	float aspectRatio;
	float nearPlane;
	float farPlane;

	Math::Matrix viewMatrix;
	Math::Matrix projectionMatrix;
};

#endif