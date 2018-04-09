#include "TextureObject.hpp"

// Only for updating obb. Other solution would be preferred
#include "ModelStorage.hpp"
#include "TextureModel.hpp"

TextureObject::TextureObject()
{
}

TextureObject::~TextureObject()
{
}

bool TextureObject::Initialize()
{
	if (!Object::Initialize())
		return false;

	m_aaba.center.x = GetPosition().x;
	m_aaba.center.y = GetPosition().z;

	float length = GetScale().length();
	m_aaba.halfSides.x = length;
	m_aaba.halfSides.y = length;

	return true;
}

bool TextureObject::Initialize(const std::string & modelName, const std::string & textureName)
{
	if (!TextureObject::Initialize())
		return false;

	m_modelName = modelName;
	m_defaultTextureName = textureName;
	m_textureName = textureName;

	return true;
}

void TextureObject::SetTextureName(const std::string & name)
{
	m_textureName = name;
}

void TextureObject::SetDefaultTexture()
{
	m_textureName = m_defaultTextureName;
}

void TextureObject::SetPosition(const Vector3f & position)
{
	Object::SetPosition(position);

	m_aaba.center.x = GetPosition().x;
	m_aaba.center.y = GetPosition().z;
}

void TextureObject::SetPosition(float x, float y, float z)
{
	SetPosition(Vector3f(x, y, z));
}

const std::string & TextureObject::GetTextureName() const
{
	return m_textureName;
}

const std::string & TextureObject::GetDefaultTextureName() const
{
	return m_defaultTextureName;
}

void TextureObject::UpdateOBB()
{
	DirectX::XMMATRIX mat = GetRotationMatrix();

	if (!ModelStorage::Get()->HasTextureModel(m_modelName))
	{
		return;
	}

	m_obb = ModelStorage::Get()->GetTextureModel(m_modelName)->GetOBB();

	m_obb.center += GetPosition();

	m_obb.vectors[0] = DirectX::XMVector3Transform(m_obb.vectors[0].XMV(), mat);
	m_obb.vectors[1] = DirectX::XMVector3Transform(m_obb.vectors[1].XMV(), mat);
	m_obb.vectors[2] = DirectX::XMVector3Transform(m_obb.vectors[2].XMV(), mat);

	m_obb.halfSides[0] *= GetScale().x;
	m_obb.halfSides[1] *= GetScale().y;
	m_obb.halfSides[2] *= GetScale().z;
}