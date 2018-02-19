#include "ModelStorage.hpp"
#include "SingleColorModel.hpp"
#include "TextureModel.hpp"

ModelStorage* Singleton<ModelStorage>::s_instance = nullptr;

ModelStorage::ModelStorage()
{
}

ModelStorage::~ModelStorage()
{
}

bool ModelStorage::LoadSingleColorModel(ID3D11Device* device, const std::string& name)
{
	if (HasSingleColorModel(name))
		return true;

	m_singleColorModels[name] = std::make_unique<SingleColorModel>();

	if (!m_singleColorModels[name]->LoadFromFile(name.c_str()))
		return false;

	if (!m_singleColorModels[name]->CreateVertexBuffer(device))
		return false;

	return true;
}

bool ModelStorage::LoadTextureModel(ID3D11Device* device, const std::string& name)
{
	if (HasTextureModel(name))
		return true;

	m_textureModels[name] = std::make_unique<TextureModel>();

	if (!m_textureModels[name]->LoadFromFile(name.c_str()))
		return false;

	if (!m_textureModels[name]->CreateVertexBuffer(device))
		return false;

	return true;
}

SingleColorModel * ModelStorage::GetSingleColorModel(const std::string& name)
{
	/*if (!HasSingleColorModel(name))
		return nullptr;*/

	return m_singleColorModels[name].get();
}

TextureModel * ModelStorage::GetTextureModel(const std::string& name)
{
	/*if (!HasTextureModel(name))
		return nullptr;*/

	return m_textureModels[name].get();
}

bool ModelStorage::HasSingleColorModel(const std::string& name) const
{
	return m_singleColorModels.find(name) != m_singleColorModels.end();
}

bool ModelStorage::HasTextureModel(const std::string& name) const
{
	return m_textureModels.find(name) != m_textureModels.end();
}
