#include "MaterialStorage.hpp"
#include "Material.hpp"

MaterialStorage* Singleton<MaterialStorage>::s_instance = nullptr;

MaterialStorage::MaterialStorage()
{
}

MaterialStorage::~MaterialStorage()
{
}

bool MaterialStorage::LoadMaterial(const std::string & name)
{
	if (HasMaterial(name))
	{
		return true;
	}

	m_materials[name] = std::make_unique<Material>();

	if (!m_materials.at(name)->Initialize(name))
	{
		return false;
	}

	return true;
}

Material * MaterialStorage::GetMaterial(const std::string & name)
{
	if (!HasMaterial(name))
	{
		return nullptr;
	}
	
	return m_materials.at(name).get();
}

bool MaterialStorage::HasMaterial(const std::string & name)
{
	return m_materials.find(name) != m_materials.end();
}