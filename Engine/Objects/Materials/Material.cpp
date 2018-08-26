#include "Material.hpp"

#include <sstream>
#include <fstream>

// For loading the default texture
//#include "../Textures/TextureStorageV2.hpp"
#include "../../FrameWork/TextureManager.hpp"

Material::Material()
{
}

Material::~Material()
{
}

bool Material::Initialize(const std::string & name, TextureManager* textureManager)
{
	std::string fileName = "../Materials/" + name;

	std::ifstream inFile(fileName);
	std::string line, attribute;

	if (!inFile.is_open())
	{
		return false;
	}

	m_name = name;

	while (std::getline(inFile, line))
	{
		std::istringstream iss(line);
		
		iss >> attribute;

		if (attribute == "newmtl")
		{
			iss >> m_nameInFile;
		}
		else if (attribute == "illum")
		{
			iss >> m_illuminationModel;
		}
		else if (attribute == "Kd")
		{
			iss >> m_diffuseReflectivity.x >> m_diffuseReflectivity.y >> m_diffuseReflectivity.z;
		}
		else if (attribute == "Ka")
		{
			iss >> m_ambientReflectivity.x >> m_ambientReflectivity.y >> m_ambientReflectivity.z;
		}
		else if (attribute == "Tf")
		{
			iss >> m_transmissionFilter.x >> m_transmissionFilter.y >> m_transmissionFilter.z;
		}
		else if (attribute == "map_Kd")
		{
			iss >> m_textureName;
			
			//if (!TextureStorage::Get()->LoadTexture(Direct3D::Get()->GetDevice(), m_textureName))
			if (m_textureIndex = textureManager->LoadTexture(m_textureName); m_textureIndex == -1)
			{
				return false;
			}
		}
		else if (attribute == "Ni")
		{
			iss >> m_refractionIndex;
		}
	}

	inFile.close();

	if (m_textureName == "" || m_textureIndex == -1)
	{
		m_textureName = "Missing.jpg";
		m_textureIndex = textureManager->LoadTexture(m_textureName);
		
		//if (!TextureStorage::Get()->LoadTexture(Direct3D::Get()->GetDevice(), m_textureName))
		if (m_textureIndex == -1)
		{
			return false;
		}
	}

	return true;
}

const std::string & Material::GetTextureName() const
{
	return m_textureName;
}

int Material::GetTextureIndex() const
{
	return m_textureIndex;
}
