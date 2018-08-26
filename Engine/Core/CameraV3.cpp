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
