#include "ModelManager.hpp"
#include "MaterialManager.hpp"
#include "../FrameWork/VertexBufferManager.hpp"

#include <sstream>
#include <fstream>

ModelManager::ModelManager() : m_vertexBufferManager(nullptr), m_materialManager(nullptr), m_nrOfModels(0)
{
}

ModelManager::~ModelManager()
{
}

bool ModelManager::Initialize(VertexBufferManager * vertexBufferManager, MaterialManager* materialManager)
{
	m_vertexBufferManager = vertexBufferManager;
	m_materialManager = materialManager;

	return true;
}

int ModelManager::LoadModel(const std::string & fileName)
{
	/*
	Check if this model has already been loaded
	*/
	if (m_nameToIndexLinker.find(fileName) != m_nameToIndexLinker.end())
	{
		return m_nameToIndexLinker.at(fileName);
	}

	/*
	Open file
	*/
	std::ifstream inFile("../Models/" + fileName);
	if (!inFile.is_open())
		return -1;

	std::string line, type;
	std::vector<Vector3f> ps;
	std::vector<Vector3f> ns;
	std::vector<Vector2f> uvs;
	std::vector<TexturedModelVertex> vertices;
	TexturedModel model;

	/*
	Copy data from file to temporary vector
	Load material and texture
	*/
	while (std::getline(inFile, line))
	{
		type = "";

		std::istringstream iss(line);

		iss >> type;

		if (type == "mtllib")
		{
			iss >> line;

			model.materialIndex = m_materialManager->LoadMaterial(line);
			if (model.materialIndex == -1)
				return -1;
		}
		else if (type == "v")
		{
			float a, b, c;
			iss >> a >> b >> c;
			ps.push_back(Vector3f(a, b, c));
		}
		else if (type == "vn")
		{
			float a, b, c;
			iss >> a >> b >> c;
			ns.push_back(Vector3f(a, b, c));
		}
		else if (type == "vt")
		{
			float a, b;
			iss >> a >> b;
			uvs.push_back(Vector2f(a, b));
		}
		else if (type == "f")
		{
			int pIdx[4];
			int nIdx[4];
			int uvIdx[4];

			int nrOfVertsOnFace = 0;

			std::string str;
			while (iss >> str)
			{
				str.replace(str.find("/"), 1, " ");
				str.replace(str.find("/"), 1, " ");

				std::istringstream stringStream2(str);

				stringStream2 >> pIdx[nrOfVertsOnFace] >> uvIdx[nrOfVertsOnFace] >> nIdx[nrOfVertsOnFace];

				pIdx[nrOfVertsOnFace]--;
				nIdx[nrOfVertsOnFace]--;
				uvIdx[nrOfVertsOnFace]--;
				nrOfVertsOnFace++;
			}

			vertices.push_back({ ps[pIdx[0]], ns[nIdx[0]], uvs[uvIdx[0]] });
			vertices.push_back({ ps[pIdx[1]], ns[nIdx[1]], uvs[uvIdx[1]] });
			vertices.push_back({ ps[pIdx[2]], ns[nIdx[2]], uvs[uvIdx[2]] });

			if (nrOfVertsOnFace == 4)
			{
				vertices.push_back({ ps[pIdx[2]], ns[nIdx[2]], uvs[uvIdx[2]] });
				vertices.push_back({ ps[pIdx[3]], ns[nIdx[3]], uvs[uvIdx[3]] });
				vertices.push_back({ ps[pIdx[0]], ns[nIdx[0]], uvs[uvIdx[0]] });
			}
		}
	}

	inFile.close();

	/*
	Copy data from temporary vector to an array
	*/
	model.nrOfVertices = vertices.size();
	model.vertices = std::make_unique<TexturedModelVertex[]>(model.nrOfVertices);

	for (unsigned int i = 0; i < model.nrOfVertices; i++)
	{
		model.vertices[i] = vertices[i];
	}

	/*
	Create the vertex buffer for this model
	Every successfully created model should have the same index as its vertex buffer, since it's a 1-to-1 relationship
	But an index is stored just in case
	*/
	unsigned int vertexSize = sizeof(TexturedModelVertex);
	model.vertexBufferIndex = m_vertexBufferManager->CreateBuffer(model.nrOfVertices * vertexSize, vertexSize, static_cast<void*>(model.vertices.get()));

	/*
	Link the file name to the index
	*/
	m_models[m_nrOfModels++] = std::move(model);
	int index = m_nrOfModels - 1;
	m_nameToIndexLinker[fileName] = index;

	return index;
}

TexturedModel * ModelManager::GetModel(int index)
{
	return index >= 0 && index < static_cast<int>(m_nrOfModels) ? &m_models[index] : nullptr;
}
