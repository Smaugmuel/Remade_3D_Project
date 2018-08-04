#include "SingleColorObject.hpp"

// Only for updating obb. Other solution would be preferred
//#include "../Models/ModelStorage.hpp"
#include "../Models/ModelStorageV2.hpp"
#include "../Models/SingleColorModel.hpp"

SingleColorObject::SingleColorObject()
{
}

SingleColorObject::~SingleColorObject()
{
}

bool SingleColorObject::Initialize(const std::string & modelName, const Vector3f & color)
{
	if (m_modelIndex = ModelStorageV2::Get()->LoadSingleColorModel(modelName); m_modelIndex == -1)
		return false;
	
	//SingleColorModel* model = modelStorage->GetSingleColorModelByName(modelName);//
	//m_modelIndex = modelStorage->GetSingleColorModelIndexByName(modelName);
	//m_modelName = modelName;
	m_color = color;

	if (!Object::Initialize())
		return false;

	return true;
}

void SingleColorObject::SetModel(const std::string & modelName)
{
	m_modelIndex = ModelStorageV2::Get()->GetSingleColorModelIndexFromName(modelName);
}

void SingleColorObject::SetColor(const Vector3f & color)
{
	m_color = color;
}

void SingleColorObject::SetColor(float r, float g, float b)
{
	SetColor(Vector3f(r, g, b));
}

int SingleColorObject::GetModelIndex() const
{
	return m_modelIndex;
}

const std::string & SingleColorObject::GetModelName() const
{
	return ModelStorageV2::Get()->GetSingleColorModelNameFromIndex(m_modelIndex);
}

const Vector3f & SingleColorObject::GetColor() const
{
	return m_color;
}

void SingleColorObject::UpdateOBB()
{
	/*if (!ModelStorage::Get()->HasSingleColorModel(m_modelName))
	{
		return;
	}
	m_obb = ModelStorage::Get()->GetSingleColorModel(m_modelName)->GetOBB();*/

	if (m_modelIndex == -1)
	{
		return;
	}

	DirectX::XMMATRIX mat = GetRotationMatrix();
	m_obb = ModelStorageV2::Get()->GetSingleColorModel(m_modelIndex)->GetOBB();

	m_obb.center += GetPosition();

	m_obb.vectors[0] = DirectX::XMVector3Transform(m_obb.vectors[0].XMV(), mat);
	m_obb.vectors[1] = DirectX::XMVector3Transform(m_obb.vectors[1].XMV(), mat);
	m_obb.vectors[2] = DirectX::XMVector3Transform(m_obb.vectors[2].XMV(), mat);

	m_obb.halfSides[0] = m_obb.defaultHalfSides[0] * GetScale().x;
	m_obb.halfSides[1] = m_obb.defaultHalfSides[1] * GetScale().y;
	m_obb.halfSides[2] = m_obb.defaultHalfSides[2] * GetScale().z;
}