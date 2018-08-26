#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include <string>
#include "../../Math/Vector3.hpp"

class TextureManager;

class Material
{
public:
	Material();
	~Material();

	bool Initialize(const std::string& name, TextureManager* textureManager);
	
	const std::string& GetTextureName() const;
	int GetTextureIndex() const;

private:
	std::string m_name;
	std::string m_nameInFile;
	unsigned int m_illuminationModel;
	Vector3f m_diffuseReflectivity;
	Vector3f m_ambientReflectivity;
	Vector3f m_transmissionFilter;
	std::string m_textureName;
	int m_textureIndex;
	unsigned int m_refractionIndex;
};

#endif