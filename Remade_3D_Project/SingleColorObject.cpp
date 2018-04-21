#include "SingleColorObject.hpp"

// Only for updating obb. Other solution would be preferred
#include "ModelStorage.hpp"
#include "SingleColorModel.hpp"

SingleColorObject::SingleColorObject()
{
}

SingleColorObject::~SingleColorObject()
{
}

bool SingleColorObject::Initialize()
{
	if (!Object::Initialize())
		return false;

	m_color = Vector3f(1, 0, 0);


	return true;
}

bool SingleColorObject::Initialize(const std::string & modelName, const Vector3f & color)
{
	if (!Object::Initialize())
		return false;

	m_modelName = modelName;
	m_color = color;

	return true;
}

void SingleColorObject::SetColor(const Vector3f & color)
{
	m_color = color;
}

void SingleColorObject::SetColor(float r, float g, float b)
{
	SetColor(Vector3f(r, g, b));
}

const Vector3f & SingleColorObject::GetColor() const
{
	return m_color;
}

void SingleColorObject::UpdateOBB()
{
	DirectX::XMMATRIX mat = GetRotationMatrix();

	if (!ModelStorage::Get()->HasSingleColorModel(m_modelName))
	{
		return;
	}

	m_obb = ModelStorage::Get()->GetSingleColorModel(m_modelName)->GetOBB();


	m_obb.center += GetPosition();

	m_obb.vectors[0] = DirectX::XMVector3Transform(m_obb.vectors[0].XMV(), mat);
	m_obb.vectors[1] = DirectX::XMVector3Transform(m_obb.vectors[1].XMV(), mat);
	m_obb.vectors[2] = DirectX::XMVector3Transform(m_obb.vectors[2].XMV(), mat);

	m_obb.halfSides[0] = m_obb.defaultHalfSides[0] * GetScale().x;
	m_obb.halfSides[1] = m_obb.defaultHalfSides[1] * GetScale().y;
	m_obb.halfSides[2] = m_obb.defaultHalfSides[2] * GetScale().z;
}