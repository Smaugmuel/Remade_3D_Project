#include "Object.hpp"

Object::Object()
{
}

Object::~Object()
{
}

void Object::InitializeBaseClass()
{
	m_position = Vector3f(0, 0, 0);
	m_rotation = Vector3f(0, 0, 0);
	m_scale = Vector3f(1, 1, 1);

	UpdateWorldMatrix();
}

void Object::Update()
{
	if (m_update_flag_world)
	{
		UpdateWorldMatrix();
	}
}

void Object::Move(const Vector3f& offset)
{
	if (offset == Vector3f(0, 0, 0))
	{
		return;
	}

	m_position += offset;

	m_update_flag_world = true;
}

void Object::Move(float x, float y, float z)
{
	Move(Vector3f(x, y, z));
}

void Object::Rotate(const Vector3f& rotation)
{
	if (rotation == Vector3f(0, 0, 0))
	{
		return;
	}
	m_rotation += rotation;
	m_update_flag_world = true;
}

void Object::Rotate(float x, float y, float z)
{
	Rotate(Vector3f(x, y, z));
}

void Object::SetPosition(const Vector3f& position)
{
	if (position == m_position)
	{
		return;
	}

	m_position = position;
	m_update_flag_world = true;
}

void Object::SetPosition(float x, float y, float z)
{
	SetPosition(Vector3f(x, y, z));
}

void Object::SetRotation(const Vector3f& rotation)
{
	if (rotation == m_rotation)
	{
		return;
	}

	m_rotation = rotation;

	m_update_flag_world = true;
}

void Object::SetRotation(float x, float y, float z)
{
	SetRotation(Vector3f(x, y, z));
}

void Object::SetScale(const Vector3f& scale)
{
	if (scale == m_scale || scale == Vector3f(0, 0, 0))
	{
		return;
	}

	m_scale = scale;

	m_update_flag_world = true;
}

void Object::SetScale(float x, float y, float z)
{
	SetScale(Vector3f(x, y, z));
}

const Vector3f& Object::GetPosition() const
{
	return m_position;
}

const Vector3f& Object::GetRotation() const
{
	return m_rotation;
}

const Vector3f& Object::GetScale() const
{
	return m_scale;
}

const DirectX::XMMATRIX Object::GetWorldMatrix() const
{
	return DirectX::XMLoadFloat4x4(&m_worldMatrix);
}

void Object::UpdateWorldMatrix()
{
	DirectX::XMMATRIX transformation =
		DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z)) *
		DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationX(m_rotation.x) * DirectX::XMMatrixRotationY(m_rotation.y) * DirectX::XMMatrixRotationZ(m_rotation.z)) *
		DirectX::XMMatrixTranspose(DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z));

	DirectX::XMStoreFloat4x4(&m_worldMatrix, transformation);
}