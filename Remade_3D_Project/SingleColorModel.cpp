#include "SingleColorModel.hpp"

#include <sstream>
#include <fstream>
#include <vector>

#include "Buffers.hpp"

#include <d3d11.h>

SingleColorModel::SingleColorModel() :
	Model(),
	m_vertices(nullptr)
{
}
SingleColorModel::~SingleColorModel()
{
	if (m_vertices)
	{
		delete[] m_vertices;
		m_vertices = nullptr;
	}
}

bool SingleColorModel::LoadFromFile(const char* fileName)
{
	std::ifstream inFile(fileName);
	std::string line, type;

	std::vector<DirectX::XMFLOAT3> positions;
	std::vector<DirectX::XMFLOAT3> normals;

	std::vector<SingleColorVertex> vertices;

	float a, b, c;
	
	while (std::getline(inFile, line))
	{
		type = "";
	
		std::istringstream stringStream(line);
	
		stringStream >> type;
	
		if (type == "mtllib")
		{
			stringStream >> line;
			//LoadMaterial();
		}
		else if (type == "v")
		{
			stringStream >> a >> b >> c;
			positions.push_back(DirectX::XMFLOAT3(a, b, c));
		}
		else if (type == "vn")
		{
			stringStream >> a >> b >> c;
			normals.push_back(DirectX::XMFLOAT3(a, b, c));
		}
		else if (type == "f")
		{
			int positionIndex[4];
			int normalIndex[4];
	
			int index = 0;
	
			std::string str;
			while (stringStream >> str)
			{
				str.replace(str.find("/"), 1, " ");
				str.replace(str.find("/"), 1, " ");
	
				std::istringstream stringStream2(str);
	
				stringStream2 >> positionIndex[index] >> normalIndex[index];
	
				positionIndex[index]--;
				normalIndex[index]--;
				index++;
			}
	
			vertices.push_back(SingleColorVertex(positions[positionIndex[0]], normals[normalIndex[0]], { 1, 0, 0 } /* m_material->GetAmbient() */));
			vertices.push_back(SingleColorVertex(positions[positionIndex[1]], normals[normalIndex[1]], { 1, 0, 0 }));
			vertices.push_back(SingleColorVertex(positions[positionIndex[2]], normals[normalIndex[2]], { 1, 0, 0 }));
			vertices.push_back(SingleColorVertex(positions[positionIndex[2]], normals[normalIndex[2]], { 1, 0, 0 }));
			vertices.push_back(SingleColorVertex(positions[positionIndex[3]], normals[normalIndex[3]], { 1, 0, 0 }));
			vertices.push_back(SingleColorVertex(positions[positionIndex[0]], normals[normalIndex[0]], { 1, 0, 0 }));
		}
	}

	// Copy into pointer
	m_nrOfVerts = vertices.size();

	m_vertices = new SingleColorVertex[m_nrOfVerts];

	for (unsigned int i = 0; i < m_nrOfVerts; i++)
	{
		m_vertices[i] = vertices[i];
	}

	return true;
}
bool SingleColorModel::CreateVertexBuffer(ID3D11Device* device)
{
	//m_vertexBuffer = Buffers::CreateVertexBuffer(device, m_nrOfVerts * sizeof(SingleColorVertex), m_vertices);
	m_vertexBuffer = Buffers::CreateDynamicVertexBuffer(device, m_nrOfVerts * sizeof(SingleColorVertex), m_vertices);

	if (!m_vertexBuffer)
	{
		return false;
	}
	return true;
}

void SingleColorModel::SetupRender(ID3D11DeviceContext * deviceContext)
{
	unsigned int vertexSize = sizeof(SingleColorVertex);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &vertexSize, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

Model* SingleColorModel::Create(ID3D11Device* device, const char* fileName)
{
	Model* model = new SingleColorModel;
	if (!model->LoadFromFile(fileName))
	{
		delete model;
		return nullptr;
	}
	if (!model->CreateVertexBuffer(device))
	{
		delete model;
		return nullptr;
	}

	return model;
}

//BaseModel* SingleColorModel::Create(const std::string& fileName, ID3D11Device* device)
//{
//	SingleColorModel* model = new SingleColorModel;
//	if (!model->LoadFromFile(fileName))
//	{
//		delete model;
//		return nullptr;
//	}
//	if (!model->CreateVertexBuffer(device))
//	{
//		delete model;
//		return nullptr;
//	}
//
//	return model;
//}