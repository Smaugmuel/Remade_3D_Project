#include "Object.hpp"

#include "MatrixEulerConversion.hpp"

Object::Object()
{
}

Object::~Object()
{
}
bool Object::Initialize()
{
	m_position = Vector3f(0, 0, 0);
	m_rotation = Vector3f(0, 0, 0);
	m_scale = Vector3f(1, 1, 1);

	UpdateTranslationMatrix();
	DirectX::XMStoreFloat4x4(&m_rotationMatrix, DirectX::XMMatrixIdentity());

	UpdateScalingMatrix();
	UpdateWorldMatrix();

	return true;
}

void Object::Update()
{
	if (m_update_translation_flag)
	{
		UpdateTranslationMatrix();
		m_update_world_flag = true;
	}
	if (m_update_rotation_from_euler_flag)
	{
		UpdateRotationMatrixFromEuler();
		m_update_world_flag = true;
	}
	if (m_update_rotation_from_matrix_flag)
	{
		UpdateRotationEulerFromMatrix();
		m_update_world_flag = true;
	}
	if (m_update_scaling_flag)
	{
		UpdateScalingMatrix();
		m_update_world_flag = true;
	}

	if (m_update_world_flag)
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

	//m_update_world_flag = true;
	m_update_translation_flag = true;
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
	
	//m_update_world_flag = true;
	m_update_rotation_from_euler_flag = true;
	//m_update_rotation_flag = true;
}

void Object::Rotate(float x, float y, float z)
{
	Rotate(Vector3f(x, y, z));
}

void Object::RotateLocal(const Vector3f & vec, float angle)
{
	DirectX::XMStoreFloat4x4(
		&m_rotationMatrix,
		DirectX::XMMatrixMultiply(
			DirectX::XMMatrixRotationAxis(vec.XMV(), -angle),
			DirectX::XMLoadFloat4x4(&m_rotationMatrix)
		)
	);

	m_update_rotation_from_matrix_flag = true;
	//m_update_world_flag = true;
}
	

void Object::RotateLocal(float x, float y, float z, float angle)
{
	RotateLocal(Vector3f(x, y, z), angle);
}

void Object::RotateGlobal(const Vector3f & vec, float angle)
{
	DirectX::XMStoreFloat4x4(
		&m_rotationMatrix,
		DirectX::XMMatrixMultiply(
			DirectX::XMLoadFloat4x4(&m_rotationMatrix),
			DirectX::XMMatrixRotationAxis(vec.XMV(), -angle)		// Negative just because
		)
	);

	m_update_rotation_from_matrix_flag = true;
	//m_update_world_flag = true;
}

void Object::RotateGlobal(float x, float y, float z, float angle)
{
	RotateGlobal(Vector3f(x, y, z), angle);
}

void Object::SetPosition(const Vector3f& position)
{
	if (position == m_position)
	{
		return;
	}

	m_position = position;
	
	//m_update_world_flag = true;
	m_update_translation_flag = true;
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

	//m_update_world_flag = true;
	m_update_rotation_from_euler_flag = true;
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

	//m_update_world_flag = true;
	m_update_scaling_flag = true;
}

void Object::SetScale(float x, float y, float z)
{
	SetScale(Vector3f(x, y, z));
}

void Object::SetModelName(const std::string & modelName)
{
	m_modelName = modelName;
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

DirectX::XMMATRIX Object::GetRotationMatrix() const
{
	return DirectX::XMLoadFloat4x4(&m_rotationMatrix);
}

DirectX::XMMATRIX Object::GetWorldMatrix() const
{
	return DirectX::XMLoadFloat4x4(&m_worldMatrix);
}

const std::string & Object::GetModelName() const
{
	return m_modelName;
}

void Object::UpdateTranslationMatrix()
{
	DirectX::XMStoreFloat4x4(&m_translationMatrix, DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z));
	m_update_translation_flag = false;
}

void Object::UpdateRotationMatrixFromEuler()
{
	m_rotationMatrix = EulerToMatrix(m_rotation);

	/*DirectX::XMMATRIX rot = DirectX::XMMatrixMultiply(
		DirectX::XMMatrixRotationX(m_rotation.x),
		DirectX::XMMatrixRotationY(m_rotation.y)
	);
	rot = DirectX::XMMatrixMultiply(
		rot,
		DirectX::XMMatrixRotationZ(m_rotation.z)
	);

	DirectX::XMStoreFloat4x4(&m_rotationMatrix, rot);*/
	
	m_update_rotation_from_euler_flag = false;
}

void Object::UpdateRotationEulerFromMatrix()
{
	m_rotation = MatrixToEuler(DirectX::XMLoadFloat4x4(&m_rotationMatrix));
	
	m_update_rotation_from_matrix_flag = false;
}

void Object::UpdateScalingMatrix()
{
	DirectX::XMStoreFloat4x4(&m_scalingMatrix, DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z));
	m_update_scaling_flag = false;
}

void Object::UpdateWorldMatrix()
{
	//DirectX::XMMATRIX transformationRot = DirectX::XMLoadFloat4x4(&m_rotationMatrix);
		
		/*DirectX::XMMatrixMultiply(
		DirectX::XMMatrixRotationX(m_rotation.x),
		DirectX::XMMatrixRotationY(m_rotation.y)
	);
	transformationRot = DirectX::XMMatrixMultiply(
		transformationRot,
		DirectX::XMMatrixRotationZ(m_rotation.z)
	);*/

	DirectX::XMMATRIX transformation = DirectX::XMMatrixMultiply(
		DirectX::XMMatrixTranspose(/*DirectX::XMLoadFloat4x4(&m_worldMatrix)*/DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z)),
		DirectX::XMMatrixTranspose(/*DirectX::XMLoadFloat4x4(&m_rotationMatrix)*//*transformationRot*/DirectX::XMLoadFloat4x4(&m_rotationMatrix))
	);
	transformation = DirectX::XMMatrixMultiply(
		transformation,
		DirectX::XMMatrixTranspose(/*DirectX::XMLoadFloat4x4(&m_scalingMatrix)*/DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z))
	);



	/*DirectX::XMMATRIX transformation =
		DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z)) *
		DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationX(m_rotation.x) * DirectX::XMMatrixRotationY(m_rotation.y) * DirectX::XMMatrixRotationZ(m_rotation.z)) *
		DirectX::XMMatrixTranspose(DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z));*/

	DirectX::XMStoreFloat4x4(&m_worldMatrix, transformation);

	m_update_world_flag = false;
}