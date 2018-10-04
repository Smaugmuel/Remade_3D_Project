#include "CameraV3.hpp"

CameraV3::CameraV3()
	: position(0, 0, 0), target(0, 0, -1), up(0, 1, 0), fov(3.14159265f), aspectRatio(1.0f), nearPlane(0.01f), farPlane(1000.0f)
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

CameraV3::CameraV3(const CameraV3 & other)
{
	*this = other;
}

CameraV3::~CameraV3()
{
}

CameraV3 & CameraV3::operator=(const CameraV3 & right)
{
	position = right.position;
	target = right.target;
	up = right.up;
	fov = right.fov;
	aspectRatio = right.aspectRatio;
	nearPlane = right.nearPlane;
	farPlane = right.farPlane;
	viewMatrix = right.viewMatrix;
	projectionMatrix = right.projectionMatrix;
	
	return *this;
}

void CameraV3::UpdateViewMatrix()
{
	viewMatrix.SetView(position, target, up);
}

void CameraV3::UpdateProjectionMatrix()
{
	projectionMatrix.SetProjection(fov, aspectRatio, nearPlane, farPlane);
}

void CameraV3::MoveForward(float amount)
{
	Vector3f dir = TargetDirection();
	position += Forward() * amount;
	target = position + dir;
}

void CameraV3::MoveRight(float amount)
{
	Vector3f dir = TargetDirection();
	position += Right() * amount;
	target = position + dir;
}

void CameraV3::MoveUp(float amount)
{
	Vector3f dir = TargetDirection();
	position += up * amount;
	target = position + dir;
}

void CameraV3::RotateUp(float angle)
{
	Vector3f direction = TargetDirection();
	Vector3f newDirection = Math::Transform(direction, 0.0f, Math::Matrix::Rotation(Right(), angle));
	target = position + newDirection;
}

void CameraV3::RotateRight(float angle)
{
	Vector3f direction = TargetDirection();
	Vector3f newDirection = Math::Transform(direction, 0.0f, Math::Matrix::Rotation(up, -angle));
	target = position + newDirection;
}

Vector3f CameraV3::TargetDirection() const
{
	return (target - position).normalized();
}

Vector3f CameraV3::Right() const
{
	return TargetDirection().crossLH(up).normalized();
}

Vector3f CameraV3::Forward() const
{
	return Right().crossRH(up).normalized();
}
