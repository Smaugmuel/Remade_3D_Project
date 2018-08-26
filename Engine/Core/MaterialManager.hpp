#ifndef MATERIAL_MANAGER_HPP
#define MATERIAL_MANAGER_HPP
#include <string>
#include <map>
#include <vector>
#include "../Math/Vector3.hpp"

class TextureManager;

struct Material
{
	std::string name;
	unsigned int illuminationModel = -1;
	Vector3f diffuseReflectivity;
	Vector3f ambientReflectivity;
	Vector3f transmissionFilter;
	std::string textureName;
	int textureIndex = -1;
	unsigned int refractionIndex = -1;
};

class MaterialManager final
{
public:
	MaterialManager();
	~MaterialManager();

	bool Initialize(TextureManager* textureManager);

	int LoadMaterial(const std::string& fileName);
	Material* GetMaterial(int index);

private:
	std::map<std::string, int> m_nameToIndexLinker;
	std::vector<Material> m_materials;

	TextureManager* m_textureManager;
};

#endif