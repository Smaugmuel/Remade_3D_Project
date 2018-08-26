#include "MaterialManager.hpp"
#include "../FrameWork/TextureManager.hpp"
#include <sstream>
#include <fstream>

MaterialManager::MaterialManager() : m_textureManager(nullptr)
{
}

MaterialManager::~MaterialManager()
{
}

bool MaterialManager::Initialize(TextureManager* textureManager)
{
	m_textureManager = textureManager;
	return true;
}

int MaterialManager::LoadMaterial(const std::string & fileName)
{
	/*
	Check if this material has already been loaded
	*/
	if (m_nameToIndexLinker.find(fileName) != m_nameToIndexLinker.end())
	{
		return m_nameToIndexLinker.at(fileName);
	}

	/*
	Open file
	*/
	std::ifstream inFile("../Materials/" + fileName);
	if (!inFile.is_open())
		return -1;

	std::string line, type;
	Material material;
	bool hasTexture = false;

	/*
	Copy data from file to material
	Load texture
	*/
	while (std::getline(inFile, line))
	{
		type = "";

		std::istringstream iss(line);

		iss >> type;

		if (type == "newmtl")
		{
			iss >> material.name;
		}
		else if (type == "illum")
		{
			iss >> material.illuminationModel;
		}
		else if (type == "Kd")
		{
			iss >> material.diffuseReflectivity.x >> material.diffuseReflectivity.y >> material.diffuseReflectivity.z;
		}
		else if (type == "Ka")
		{
			iss >> material.ambientReflectivity.x >> material.ambientReflectivity.y >> material.ambientReflectivity.z;
		}
		else if (type == "Tf")
		{
			iss >> material.transmissionFilter.x >> material.transmissionFilter.y >> material.transmissionFilter.z;
		}
		else if (type == "map_Kd")
		{
			hasTexture = true;
			iss >> material.textureName;

			material.textureIndex = m_textureManager->LoadTexture(material.textureName);
			if (material.textureIndex == -1)
				return -1;
		}
		else if (type == "Ni")
		{
			iss >> material.refractionIndex;
		}
	}

	inFile.close();

	/*
	If this material has no texture, add the "missing" texture
	*/
	if (!hasTexture)
	{
		material.textureName = "Missing.jpg";
		material.textureIndex = m_textureManager->LoadTexture(material.textureName);

		/*
		If the "missing" texture failed to load, break program
		*/
		if (material.textureIndex == -1)
			return -1;
	}

	m_materials.push_back(material);
	int index = m_materials.size() - 1;
	m_nameToIndexLinker[fileName] = index;
	return index;
}

Material * MaterialManager::GetMaterial(int index)
{
	return index >= 0 && index < static_cast<int>(m_materials.size()) ? &m_materials[index] : nullptr;
}
