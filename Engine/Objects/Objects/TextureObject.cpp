#include "TextureObject.hpp"

// For updating obb and retrieving default texture. Other solution would be preferred
//#include "../Models/ModelStorage.hpp"
//#include "../Models/ModelStorageV2.hpp"
#include "../Models/TextureModel.hpp"

// For retrieving default texture
//#include "../Materials/MaterialStorage.hpp"
//#include "../Materials/MaterialStorageV2.hpp"
//#include "../Materials/Material.hpp"
//
//#include "../Textures/TextureStorageV2.hpp"

TextureObject::TextureObject()
{
}

TextureObject::~TextureObject()
{
}

bool TextureObject::Initialize(const std::string & modelName)
{
	/*ModelStorageV2* modelStorage = ModelStorageV2::Get();
	MaterialStorageV2* materialStorage = MaterialStorageV2::Get();*/

	// Abort if the model or material fails to load
	//if (m_modelIndex = modelStorage->LoadTextureModel(modelName); m_modelIndex == -1)
		return false;

	/*if (m_materialIndex = materialStorage->LoadMaterial(modelStorage->GetTextureModel(m_modelIndex)->GetMaterialName()))
		return false;
	Material* material = materialStorage->GetMaterial(model->GetMaterialName());*/
	

	//m_modelIndex = modelStorage->GetTextureModelIndexByName(modelName);
	//m_defaultTextureIndex = materialStorage->GetMaterial(modelStorage->GetTextureModel(m_modelIndex)->GetMaterialIndex())->GetTextureIndex();
	m_textureIndex = m_defaultTextureIndex;

	if (!Object::Initialize())
		return false;

	return true;
}

void TextureObject::SetModel(int index)
{
	/*if (index >= 0 && static_cast<unsigned int>(index) < ModelStorageV2::Get()->GetNrOfTextureModels())
	{
		m_modelIndex = index;
	}*/
}

void TextureObject::SetModelName(const std::string & name)
{
	/*switch (int index = ModelStorageV2::Get()->GetTextureModelIndexFromName(name))
	{
	case -1:
		m_modelIndex = ModelStorageV2::Get()->GetTextureModelIndexFromName("cube_uv.obj");
		break;
	default:
		m_modelIndex = index;
		break;
	}*/
}

void TextureObject::SetTexture(int index)
{
	/*if (index >= 0 && static_cast<unsigned int>(index) < TextureStorageV2::Get()->GetNrOfTextures())
	{
		m_modelIndex = index;
	}*/
}

void TextureObject::SetTextureName(const std::string & name)
{
	//m_textureIndex = TextureStorageV2::Get()->GetIndexByName(name);
	//m_textureName = name;
}

void TextureObject::SetDefaultTexture()
{
	m_textureIndex = m_defaultTextureIndex;
	//m_textureName = m_defaultTextureName;
}

void TextureObject::SetPosition(const Vector3f & position)
{
	Object::SetPosition(position);

	/*m_aaba.center.x = GetPosition().x;
	m_aaba.center.y = GetPosition().z;*/
}

void TextureObject::SetPosition(float x, float y, float z)
{
	SetPosition(Vector3f(x, y, z));
}

int TextureObject::GetModelIndex() const
{
	return m_modelIndex;
}

int TextureObject::GetTextureIndex() const
{
	return m_textureIndex;
}

int TextureObject::GetDefaultTextureIndex() const
{
	return m_defaultTextureIndex;
}

const std::string & TextureObject::GetModelName() const
{
	return "";
	//return ModelStorageV2::Get()->GetTextureModelNameFromIndex(m_modelIndex);
}

const std::string & TextureObject::GetTextureName() const
{
	return "";
	//return TextureStorageV2::Get()->GetNameByIndex(m_textureIndex);
}

const std::string & TextureObject::GetDefaultTextureName() const
{
	return "";
	//return TextureStorageV2::Get()->GetNameByIndex(m_defaultTextureIndex);
}

void TextureObject::UpdateOBB()
{
	DirectX::XMMATRIX mat = GetRotationMatrix();

	/*if (!ModelStorage::Get()->HasTextureModel(m_modelName))
	{
		return;
	}

	m_obb = ModelStorage::Get()->GetTextureModel(m_modelName)->GetOBB();*/
	if (m_modelIndex == -1)
	{
		return;
	}

	//m_obb = ModelStorageV2::Get()->GetTextureModel(m_modelIndex)->GetOBB();

	m_obb.center += GetPosition();

	/*m_obb.vectors[0] = DirectX::XMVector3Transform(m_obb.vectors[0].XMV(), mat);
	m_obb.vectors[1] = DirectX::XMVector3Transform(m_obb.vectors[1].XMV(), mat);
	m_obb.vectors[2] = DirectX::XMVector3Transform(m_obb.vectors[2].XMV(), mat);*/

	m_obb.halfSides[0] = m_obb.defaultHalfSides[0] * GetScale().x;
	m_obb.halfSides[1] = m_obb.defaultHalfSides[1] * GetScale().y;
	m_obb.halfSides[2] = m_obb.defaultHalfSides[2] * GetScale().z;
}