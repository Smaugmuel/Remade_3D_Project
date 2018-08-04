#include "ModelStorageV2.hpp"
#include "SingleColorModel.hpp"
#include "TextureModel.hpp"
#include "LineModel.hpp"

ModelStorageV2* Singleton<ModelStorageV2>::s_instance = nullptr;

ModelStorageV2::ModelStorageV2()
{
	LoadTextureModel("cube_uv.obj");
}
ModelStorageV2::~ModelStorageV2()
{
}

int ModelStorageV2::LoadSingleColorModel(const std::string & name)
{
	// Return the index if the model already exists
	if (int index = GetSingleColorModelIndexFromName(name); index != -1)
	{
		return index;
	}

	// Add a model
	m_singleColorModels.push_back(S_C_Model());
	S_C_Model* model = &m_singleColorModels.back();
	model->model = std::make_unique<SingleColorModel>();

	// Abort if the model fails to load
	if (!model->model->LoadFromFile(std::string("../Models/" + name).c_str()))
	{
		m_singleColorModels.pop_back();
		return -1;
	}

	// Abort if the model fails to create its vertex buffer
	if (!model->model->CreateVertexBuffer())
	{
		m_singleColorModels.pop_back();
		return -1;
	}

	// Abort if the model fails to create its OBB
	if (!model->model->CreateOBB())
	{
		m_singleColorModels.pop_back();
		return -1;
	}

	model->name = name;

	return m_singleColorModels.size() - 1;
}
int ModelStorageV2::LoadTextureModel(const std::string & name)
{
	if (int index = GetTextureModelIndexFromName(name); index != -1)
		return index;

	// Add a model
	m_textureModels.push_back(T_Model());
	T_Model* model = &m_textureModels.back();
	model->model = std::make_unique<TextureModel>();

	// Abort if the model fails to load
	if (!model->model->LoadFromFile(std::string("../Models/" + name).c_str()))
	{
		m_textureModels.pop_back();
		return -1;
	}

	// Abort if the model fails to create its vertex buffer
	if (!model->model->CreateVertexBuffer())
	{
		m_textureModels.pop_back();
		return -1;
	}

	// Abort if the model fails to create its OBB
	if (!model->model->CreateOBB())
	{
		m_textureModels.pop_back();
		return -1;
	}

	model->name = name;

	return m_textureModels.size() - 1;
}
bool ModelStorageV2::CreateLineModel()
{
	if (m_lineModel != nullptr)
		return true;
	
	m_lineModel = std::make_unique<LineModel>();

	if (!m_lineModel->CreateVertexBuffer())
	{
		m_lineModel.reset();
		return false;
	}

	return true;
}

int ModelStorageV2::GetSingleColorModelIndexFromName(const std::string & name)
{
	unsigned int n = m_singleColorModels.size();
	for (unsigned int i = 0; i < n; i++)
	{
		if (m_singleColorModels[i].name == name)
		{
			return i;
		}
	}

	return -1;
}
int ModelStorageV2::GetTextureModelIndexFromName(const std::string & name)
{
	unsigned int n = m_textureModels.size();
	for (unsigned int i = 0; i < n; i++)
	{
		if (m_textureModels[i].name == name)
		{
			return i;
		}
	}

	return -1;
}
const std::string & ModelStorageV2::GetSingleColorModelNameFromIndex(int index)
{
	return index >= 0 && static_cast<unsigned int>(index) < m_singleColorModels.size() ? m_singleColorModels[index].name : "";
}
const std::string & ModelStorageV2::GetTextureModelNameFromIndex(int index)
{
	return index >= 0 && static_cast<unsigned int>(index) < m_textureModels.size() ? m_textureModels[index].name : "";
}

SingleColorModel * ModelStorageV2::GetSingleColorModel(int index)
{
	return index >= 0 && static_cast<unsigned int>(index) < m_singleColorModels.size() ? m_singleColorModels[index].model.get() : nullptr;
}
//SingleColorModel * ModelStorageV2::GetSingleColorModelByName(const std::string & name)
//{
//	return GetSingleColorModelByIndex(GetSingleColorModelIndexByName(name));
//}
TextureModel * ModelStorageV2::GetTextureModel(int index)
{
	return index >= 0 && static_cast<unsigned int>(index) < m_textureModels.size() ? m_textureModels[index].model.get() : nullptr;
}
//TextureModel * ModelStorageV2::GetTextureModelByName(const std::string & name)
//{
//	return GetTextureModelByIndex(GetTextureModelIndexByName(name));
//}

LineModel * ModelStorageV2::GetLineModel()
{
	return m_lineModel.get();
}

unsigned int ModelStorageV2::GetNrOfSingleColorModels() const
{
	return m_singleColorModels.size();
}
unsigned int ModelStorageV2::GetNrOfTextureModels() const
{
	return m_textureModels.size();
}