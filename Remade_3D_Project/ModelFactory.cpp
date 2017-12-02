#include "ModelFactory.hpp"
#include "SingleColorModel.hpp"
#include <d3d11.h>

#include <fstream>
#include <sstream>

//ModelFactory* Singleton<ModelFactory>::s_instance = nullptr;

//ModelFactory::ModelFactory() : Singleton<ModelFactory>()
//{
//	//Register("SingleColor", &SingleColorModel::Create);
//}
//
//ModelFactory::~ModelFactory()
//{
//}

bool ModelFactory::CheckIfModelHasTexcoords(const char* modelFileName)
{
	std::ifstream fin(modelFileName);
	std::string line;
	bool foundTexcoords = false;

	while (std::getline(fin, line))
	{
		std::string firstCharacter;
		std::istringstream sstream(line);

		sstream >> firstCharacter;

		if (firstCharacter == "vt")
		{
			foundTexcoords = true;
			break;
		}
		else if (firstCharacter == "vn")
		{
			// vn comes after vt in a .obj file
			// If this statement is fulfilled before the above, there were no texcoords
			// Then the rest of the file does not need to be read
			break;
		}
	}

	fin.close();

	return foundTexcoords;
}
//
//void ModelFactory::Register(const std::string& modelType, CreateFunctionPointer function)
//{
//	m_modelMap[modelType] = function;
//}

Model* ModelFactory::Create(const char* modelFileName, ID3D11Device* device)
{
	Model* model = nullptr;

	if (ModelFactory::CheckIfModelHasTexcoords(modelFileName))
	{
		// Create texture model
	}
	else
	{
		model = new SingleColorModel;
	}

	if (model)
	{
		if (!model->LoadFromFile(modelFileName))
		{
			delete model;
			model = nullptr;
		}
		if (!model->CreateVertexBuffer(device))
		{
			delete model;
			model = nullptr;
		}
	}

	return model;


	/*ModelMap::iterator it = m_modelMap.find(modelType);

	if (it == m_modelMap.end())
	{
		return nullptr;
	}
	
	return it->second(modelFileName, device);*/
}