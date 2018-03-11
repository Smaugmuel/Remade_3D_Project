#include "Camera.hpp"
#include <math.h>
#include "WindowSettings.hpp"

Camera::Camera()
{

}
Camera::Camera(const Camera& camera)
{
	*this = camera;
}
Camera::~Camera()
{
	/*if (m_frustum)
	{
		delete m_frustum;
		m_frustum = nullptr;
	}*/
}

void Camera::operator=(const Camera& camera)
{
	m_position = camera.m_position;
	m_target = camera.m_target;
	m_up = camera.m_up;
	
	m_FOV = camera.m_FOV;
	m_dimensions = camera.m_dimensions;
	m_nearPlane = camera.m_nearPlane;
	m_farPlane = camera.m_farPlane;

	m_view = camera.m_view;
	m_projection = camera.m_projection;
	m_orthographic = camera.m_orthographic;

	// Matrices were copied as well
	m_update_flag_view = false;
	m_update_flag_proj = false;
}

void Camera::Initialize()
{
	m_position = Vector3f(0, 0, 0);
	m_target = Vector3f(0, 0, 1 );
	m_up = Vector3f(0, 1, 0 );

	m_FOV = 3.1415927f * 0.5f;
	m_dimensions.x = WNDW;
	m_dimensions.y = WNDH;
	m_nearPlane = 0.1f;
	m_farPlane = 1000.0f;

	UpdateViewMatrix();
	UpdateProjectionMatrix();
	UpdateOrthographicMatrix();

	//m_frustum = new Frustum;
}

void Camera::Update()
{
	if (m_update_flag_view)
	{
		UpdateViewMatrix();
		m_update_flag_view = false;
	}
	if (m_update_flag_proj)
	{
		UpdateProjectionMatrix();
		UpdateOrthographicMatrix();
		m_update_flag_proj = false;
	}
}
void Camera::UpdateViewMatrix()
{
	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(
		m_position.XMV(),
		m_target.XMV(),
		m_up.XMV());

	DirectX::XMStoreFloat4x4(&m_view, view);
}
void Camera::UpdateProjectionMatrix()
{
	DirectX::XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(
		m_FOV,
		m_dimensions.x / m_dimensions.y,
		m_nearPlane,
		m_farPlane);

	DirectX::XMStoreFloat4x4(&m_projection, projection);
}
void Camera::UpdateOrthographicMatrix()
{
	DirectX::XMMATRIX orthographic = DirectX::XMMatrixOrthographicLH(
		m_dimensions.x * 0.05f,
		m_dimensions.y * 0.05f,
		m_nearPlane,
		m_farPlane);

	DirectX::XMStoreFloat4x4(&m_orthographic, orthographic);
}

void Camera::Move(const Vector3f& direction)
{
	SetPosition(m_position + direction);
}
void Camera::MoveForward(float amount)
{
	Move(GetForward() * amount);
}
void Camera::MoveRight(float amount)
{
	Move(GetRight() * amount);
}
void Camera::MoveUp(float amount)
{
	Move(Vector3f(0.0f, amount, 0.0f));
}
void Camera::Rotate(const Vector3f& axis, float angle)
{
	DirectX::XMVECTOR axisVector = axis.XMV();

	// Zero vector, or no rotation
	if (DirectX::XMVector3Equal(axisVector, DirectX::XMVectorZero()) || angle == 0.0f)
		return;

	// Direction to target after rotation
	DirectX::XMVECTOR newTargetDirection =
		DirectX::XMVector3Transform(
			GetTargetDirection().XMV(),
			DirectX::XMMatrixRotationAxis(
				axisVector,
				angle));

	m_target = To_XMFLOAT3(DirectX::XMVectorAdd(m_position.XMV(), newTargetDirection));

	m_update_flag_view = true;
}

void Camera::RotateUp(float angle)
{
	Rotate(GetRight(), angle);
}

void Camera::RotateRight(float angle)
{
	Rotate(Vector3f(0.0f, 1.0f, 0.0f), angle);
}

void Camera::SetPosition(const Vector3f& position)
{
	// Same position
	if (position == m_position)
		return;

	// Retain target direction
	Vector3f toTarget = GetTargetDirection();
	m_position = position;
	m_target = position + toTarget;

	m_update_flag_view = true;
}
void Camera::SetPosition(float x, float y, float z)
{
	SetPosition(Vector3f(x, y, z));
}
void Camera::SetTarget(float x, float y, float z)
{
	SetTarget(Vector3f(x, y, z));
}
void Camera::SetTarget(const Vector3f& target)
{
	if (target == m_target || target == m_position)
	{
		return;
	}

	m_target = target;
	m_update_flag_view = true;
}

void Camera::SetFOV(float FOV)
{
	if (FOV == m_FOV)
		return;

	m_FOV = FOV;

	m_update_flag_proj = true;
}
void Camera::SetDimensions(const Vector2f & dimensions)
{
	m_dimensions = dimensions;
	m_update_flag_proj = true;
}
void Camera::SetNearPlane(float nearPlane)
{
	if (nearPlane == m_nearPlane)
		return;

	m_nearPlane = nearPlane;

	m_update_flag_proj = true;
}
void Camera::SetFarPlane(float farPlane)
{
	if (farPlane == m_farPlane)
		return;

	m_farPlane = farPlane;

	m_update_flag_proj = true;
}

//void Camera::SetUnaltered()
//{
//	m_wasAltered = false;
//}

//void Camera::CreateFrustum()
//{
//	m_frustum->CreateFrustum(m_view, m_projection, m_position, m_FOV, m_farPlane);
//}

const Vector3f& Camera::GetPosition() const
{
	return m_position;
}
const Vector3f& Camera::GetTarget() const
{
	return m_target;
}

float Camera::GetFOV() const
{
	return m_FOV;
}
const Vector2f & Camera::GetDimensions() const
{
	return m_dimensions;
}
float Camera::GetNearPlane() const
{
	return m_nearPlane;
}
float Camera::GetFarPlane() const
{
	return m_farPlane;
}

const Vector3f Camera::GetTargetDirection() const
{
	return (m_target - m_position).normalized();
}
const Vector3f Camera::GetForward() const
{
	// DirectX is left handed
	return Vector3f(0, 1, 0).crossLH(GetRight()).normalized();
}
const Vector3f Camera::GetRight() const
{
	// DirectX is left handed
	return GetTargetDirection().crossLH(Vector3f(0, 1, 0)).normalized();
}

const DirectX::XMMATRIX Camera::GetViewMatrix() const
{
	return DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&m_view));
}
const DirectX::XMMATRIX Camera::GetProjectionMatrix() const
{
	return DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&m_projection));
}

const DirectX::XMMATRIX Camera::GetOrthogonalMatrix() const
{
	return DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&m_orthographic));
}

//bool Camera::GetWasAltered() const
//{
//	return m_wasAltered;
//}
//
//Frustum* Camera::GetViewFrustum()
//{
//	return m_frustum;
//}

DirectX::XMVECTOR To_XMVECTOR(DirectX::XMFLOAT3 v)
{
	return DirectX::XMLoadFloat3(&v);
}
DirectX::XMFLOAT3 To_XMFLOAT3(DirectX::XMVECTOR v)
{
	DirectX::XMFLOAT3 temp;
	DirectX::XMStoreFloat3(&temp, v);
	return temp;
}