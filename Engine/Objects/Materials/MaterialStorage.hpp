#ifndef MATERIAL_STORAGE_HPP
#define MATERIAL_STORAGE_HPP
#include "../../Misc/Singleton.hpp"
#include <map>
#include <string>
#include <memory>

class Material;

class MaterialStorage final : public Singleton<MaterialStorage>
{
	friend class Singleton<MaterialStorage>;
	MaterialStorage();
	~MaterialStorage();

public:
	bool LoadMaterial(const std::string& name);

	Material* GetMaterial(const std::string& name);

	bool HasMaterial(const std::string& name);

private:
	std::map<std::string, std::unique_ptr<Material>> m_materials;
};

#endif