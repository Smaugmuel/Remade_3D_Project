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

	std::string fullFileName = "../Models/" + name;

	if (!m_singleColorModels.at(name).get()->LoadFromFile(fullFileName.c_str()))
		return false;

	if (!m_singleColorModels.at(name).get()->CreateVertexBuffer(device))
		return false;

	if (!m_singleColorModels.at(name).get()->CreateOBB())
		return false;

	return true;
}

bool ModelStorage::LoadTextureModel(ID3D11Device* device, const std::string& name)
{
	if (HasTextureModel(name))
		return true;

	m_textureModels[name] = std::make_unique<TextureModel>();

	std::string fullFileName = "../Models/" + name;

	if (!m_textureModels.at(name).get()->LoadFromFile(fullFileName.c_str()))
		return false;

	if (!m_textureModels.at(name).get()->CreateVertexBuffer(device))
		return false;

	if (!m_textureModels.at(name).get()->CreateOBB())
		return false;

	return true;
}

SingleColorModel * ModelStorage::GetSingleColorModel(const std::string& name)
{
	/*if (!HasSingleColorModel(name))
		return nullptr;*/

	return m_singleColorModels.at(name).get();
}

TextureModel * ModelStorage::GetTextureModel(const std::string& name)
{
	/*if (!HasTextureModel(name))
		return nullptr;*/

	return m_textureModels.at(name).get();
}

bool ModelStorage::HasSingleColorModel(const std::string& name) const
{
	return m_singleColorModels.find(name) != m_singleColorModels.end();
}

bool ModelStorage::HasTextureModel(const std::string& name) const
{
	return m_textureModels.find(name) != m_textureModels.end();
}

unsigned int ModelStorage::GetNrOfSingleColorModels() const
{
	return m_singleColorModels.size();
}

unsigned int ModelStorage::GetNrOfTextureModels() const
{
	return m_textureModels.size();
}

const std::string & ModelStorage::GetSingleColorModelName(unsigned int i) const
{
	if (i < 0 || i > m_singleColorModels.size() - 1)
	{
		return "";
	}

	std::map<std::string, std::unique_ptr<SingleColorModel>>::const_iterator it = m_singleColorModels.begin();
	std::map<std::string, std::unique_ptr<SingleColorModel>>::const_iterator end = m_singleColorModels.end();
	std::string name;
	unsigned int counter = 0;

	for (it; it != end; ++it)
	{
		if (counter++ == i)
		{
			name = it->first;
			break;
		}
	}

	return name;
}

const std::string & ModelStorage::GetTextureModelName(unsigned int i) const
{
	if (i < 0 || i > m_textureModels.size() - 1)
	{
		return "";
	}

	std::map<std::string, std::unique_ptr<TextureModel>>::const_iterator it = m_textureModels.begin();
	std::map<std::string, std::unique_ptr<TextureModel>>::const_iterator end = m_textureModels.end();
	std::string name;
	unsigned int counter = 0;

	for (it; it != end; ++it)
	{
		if (counter++ == i)
		{
			name = it->first;
			break;
		}
	}

	return name;
}
