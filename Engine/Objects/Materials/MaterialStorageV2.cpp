#include "MaterialStorageV2.hpp"
#include "Material.hpp"

//MaterialStorageV2* Singleton<MaterialStorageV2>::s_instance = nullptr;

MaterialStorageV2::MaterialStorageV2()
{
}
MaterialStorageV2::~MaterialStorageV2()
{
}

bool MaterialStorageV2::Initialize(TextureManager * textureManager)
{
	m_textureManager = textureManager;
	return true;
}

int MaterialStorageV2::LoadMaterial(const std::string & name)
{
	// Return the index if the material already exists
	if (int index = GetIndexFromName(name); index != -1)
	{
		return index;
	}

	// Add a material
	m_materials.push_back(Mat());
	Mat* material = &m_materials.back();
	material->material = std::make_unique<Material>();

	// Abort if the material fails to load
	if (!material->material->Initialize(name, m_textureManager))
	{
		m_materials.pop_back();
		return -1;
	}

	material->name = name;

	return m_materials.size() - 1;
}

int MaterialStorageV2::GetIndexFromName(const std::string & name)
{
	unsigned int n = m_materials.size();
	for (unsigned int i = 0; i < n; i++)
	{
		if (m_materials[i].name == name)
		{
			return i;
		}
	}

	return -1;
}
const std::string & MaterialStorageV2::GetNameFromIndex(int index)
{
	return index >= 0 && static_cast<unsigned int>(index) < m_materials.size() ? m_materials[index].name : "";
}

Material * MaterialStorageV2::GetMaterial(int index)
{
	return index >= 0 && static_cast<unsigned int>(index) < m_materials.size() ? m_materials[index].material.get() : nullptr;
}