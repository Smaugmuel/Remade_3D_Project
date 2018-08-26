#ifndef MATERIAL_STORAGE_V2_HPP
#define MATERIAL_STORAGE_V2_HPP
//#include "../../Misc/Singleton.hpp"

#include <vector>
#include <string>
#include <memory>

class Material;
class TextureManager;

class MaterialStorageV2 final //: public Singleton<MaterialStorageV2>
{
public:
	MaterialStorageV2();
	~MaterialStorageV2();

	bool Initialize(TextureManager* textureManager);

	int LoadMaterial(const std::string& name);

	int GetIndexFromName(const std::string& name);
	const std::string& GetNameFromIndex(int index);

	Material* GetMaterial(int index);

private:
	//friend class Singleton<MaterialStorageV2>;

	struct Mat
	{
		std::string name = "";
		std::unique_ptr<Material> material = nullptr;
	};


	std::vector<Mat> m_materials;
	TextureManager* m_textureManager;
};

#endif