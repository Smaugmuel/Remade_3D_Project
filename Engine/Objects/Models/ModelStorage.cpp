#include "ModelStorage.hpp"
#include "SingleColorModel.hpp"
#include "TextureModel.hpp"
#include "LineModel.hpp"

ModelStorage* Singleton<ModelStorage>::s_instance = nullptr;

ModelStorage::ModelStorage()
{
}

ModelStorage::~ModelStorage()
{
}

bool ModelStorage::LoadSingleColorModel(const std::string& name)
{
	if (HasSingleColorModel(name))
		return true;

	m_singleColorModels[name] = std::make_unique<SingleColorModel>();

	std::string fullFileName = "../Models/" + name;

	if (!m_singleColorModels.at(name).get()->LoadFromFile(fullFileName.c_str()))
	{
		m_singleColorModels.erase(name);
		return false;
	}

	if (!m_singleColorModels.at(name).get()->CreateVertexBuffer())
	{
		m_singleColorModels.erase(name);
		return false;
	}

	if (!m_singleColorModels.at(name).get()->CreateOBB())
	{
		m_singleColorModels.erase(name);
		return false;
	}

	return true;
}

bool ModelStorage::LoadTextureModel(const std::string& name)
{
	if (HasTextureModel(name))
		return true;

	m_textureModels[name] = std::make_unique<TextureModel>();

	std::string fullFileName = "../Models/" + name;

	if (!m_textureModels.at(name).get()->LoadFromFile(fullFileName.c_str()))
	{
		m_textureModels.erase(name);
		return false;
	}

	if (!m_textureModels.at(name).get()->CreateVertexBuffer())
	{
		m_textureModels.erase(name);
		return false;
	}

	if (!m_textureModels.at(name).get()->CreateOBB())
	{
		m_textureModels.erase(name);
		return false;
	}

	return true;
}

bool ModelStorage::CreateLineModel()
{
	if (HasLineModel())
		return true;
	
	m_lineModel = std::make_unique<LineModel>();

	if (!m_lineModel->CreateVertexBuffer())
	{
		m_lineModel.reset();
		return false;
	}

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

LineModel * ModelStorage::GetLineModel()
{
	return m_lineModel.get();
}

bool ModelStorage::HasSingleColorModel(const std::string& name) const
{
	return m_singleColorModels.find(name) != m_singleColorModels.end();
}

bool ModelStorage::HasTextureModel(const std::string& name) const
{
	return m_textureModels.find(name) != m_textureModels.end();
}

bool ModelStorage::HasLineModel() const
{
	return m_lineModel != nullptr;
}

unsigned int ModelStorage::GetNrOfSingleColorModels() const
{
	return m_singleColorModels.size();
}

unsigned int ModelStorage::GetNrOfTextureModels() const
{
	return m_textureModels.size();
}

std::string ModelStorage::GetSingleColorModelName(unsigned int i) const
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

std::string ModelStorage::GetTextureModelName(unsigned int i) const
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
