#include "ObjectV3.hpp"
#include "SceneManagerV3.hpp"

ObjectV3::ObjectV3()
{
	m_isWorldMatrixUpdated = false;
	m_scale = Vector3f(1, 1, 1);
}

ObjectV3::~ObjectV3()
{
}

void ObjectV3::SetPosition(Vector3f position)
{
	m_position = position;
	m_translationMatrix.SetTranslation(m_position);

	m_isWorldMatrixUpdated = false;
}

void ObjectV3::SetScale(Vector3f scale)
{
	m_scale = scale;
	m_scaleMatrix.SetScale(scale);
	m_isWorldMatrixUpdated = false;
}

void ObjectV3::SetModel(int modelIndex)
{
	m_modelIndex = modelIndex;
	//m_sceneManager->SortObjectInArray(m_index);
}

void ObjectV3::SetTexture(int textureIndex)
{
	m_textureIndex = textureIndex;
	//m_sceneManager->SortObjectInArray(m_index);
}

void ObjectV3::Rotate(Vector3f axis, float angle)
{
	m_rotationMatrix = Math::Matrix::Rotation(axis, angle) * m_rotationMatrix;
	m_isWorldMatrixUpdated = false;
}

void ObjectV3::Rotate(const Math::Matrix & rotationMatrix)
{
	m_rotationMatrix = rotationMatrix * m_rotationMatrix;
	m_isWorldMatrixUpdated = false;
}

Vector3f ObjectV3::GetPosition() const noexcept
{
	return m_position;
}

Vector3f ObjectV3::GetScale() const noexcept
{
	return m_scale;
}

int ObjectV3::GetObjectIndex() const noexcept
{
	return m_index;
}

int ObjectV3::GetModelIndex() const noexcept
{
	return m_modelIndex;
}

int ObjectV3::GetTextureIndex() const noexcept
{
	return m_textureIndex;
}

const Math::Matrix & ObjectV3::GetWorldMatrix() const noexcept
{
	if (!m_isWorldMatrixUpdated)
	{
		m_worldMatrix = m_scaleMatrix * m_rotationMatrix * m_translationMatrix;
		m_isWorldMatrixUpdated = true;
	}
	return m_worldMatrix;
}
