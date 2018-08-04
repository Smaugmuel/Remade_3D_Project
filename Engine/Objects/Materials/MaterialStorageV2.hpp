#ifndef MATERIAL_STORAGE_V2_HPP
#define MATERIAL_STORAGE_V2_HPP
#include "../../Misc/Singleton.hpp"

#include <vector>
#include <string>
#include <memory>

class Material;

class MaterialStorageV2 final : public Singleton<MaterialStorageV2>
{
public:
	int LoadMaterial(const std::string& name);

	int GetIndexFromName(const std::string& name);
	const std::string& GetNameFromIndex(int index);

	Material* GetMaterial(int index);

private:
	friend class Singleton<MaterialStorageV2>;

	struct Mat
	{
		std::string name = "";
		std::unique_ptr<Material> material = nullptr;
	};

	MaterialStorageV2();
	~MaterialStorageV2();

	std::vector<Mat> m_materials;
};

#endif