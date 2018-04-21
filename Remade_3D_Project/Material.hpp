#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include <string>
#include "Vector3.hpp"

class Material
{
public:
	Material();
	~Material();

	bool Initialize(const std::string& name);
	
	const std::string& GetTextureName() const;

private:
	std::string m_name;
	std::string m_nameInFile;
	unsigned int m_illuminationModel;
	Vector3f m_diffuseReflectivity;
	Vector3f m_ambientReflectivity;
	Vector3f m_transmissionFilter;
	std::string m_textureName;
	unsigned int m_refractionIndex;
};

#endif