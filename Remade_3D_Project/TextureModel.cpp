#include "TextureModel.hpp"

#include <sstream>
#include <fstream>
#include <vector>

#include "Buffers.hpp"

#include <d3d11.h>

TextureModel::TextureModel() :
	Model(),
	m_vertices(nullptr)
{
}

TextureModel::~TextureModel()
{
	if (m_vertices)
	{
		delete[] m_vertices;
		m_vertices = nullptr;
	}
}

bool TextureModel::LoadFromFile(const char * fileName)
{
	std::ifstream inFile(fileName);
	std::string line, type;

	std::vector<DirectX::XMFLOAT3> positions;
	std::vector<DirectX::XMFLOAT3> normals;
	std::vector<DirectX::XMFLOAT2> uvs;

	std::vector<TextureVertex> vertices;

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
		else if (type == "vt")
		{
			stringStream >> a >> b;
			uvs.push_back(DirectX::XMFLOAT2(a, b));
		}
		else if (type == "f")
		{
			int positionIndex[4];
			int normalIndex[4];
			int uvIndex[4];

			int nrOfVertsOnFace = 0;

			std::string str;
			while (stringStream >> str)
			{
				str.replace(str.find("/"), 1, " ");
				str.replace(str.find("/"), 1, " ");

				std::istringstream stringStream2(str);

				stringStream2 >> positionIndex[nrOfVertsOnFace] >> uvIndex[nrOfVertsOnFace] >> normalIndex[nrOfVertsOnFace];

				positionIndex[nrOfVertsOnFace]--;
				normalIndex[nrOfVertsOnFace]--;
				uvIndex[nrOfVertsOnFace]--;
				nrOfVertsOnFace++;
			}

			vertices.push_back(TextureVertex(positions[positionIndex[0]], normals[normalIndex[0]], uvs[uvIndex[0]]));
			vertices.push_back(TextureVertex(positions[positionIndex[1]], normals[normalIndex[1]], uvs[uvIndex[1]]));
			vertices.push_back(TextureVertex(positions[positionIndex[2]], normals[normalIndex[2]], uvs[uvIndex[2]]));
					
			if (nrOfVertsOnFace == 4)
			{
				vertices.push_back(TextureVertex(positions[positionIndex[2]], normals[normalIndex[2]], uvs[uvIndex[2]]));
				vertices.push_back(TextureVertex(positions[positionIndex[3]], normals[normalIndex[3]], uvs[uvIndex[3]]));
				vertices.push_back(TextureVertex(positions[positionIndex[0]], normals[normalIndex[0]], uvs[uvIndex[0]]));
			}
		}
	}

	// Copy into pointer
	m_nrOfVerts = vertices.size();

	m_vertices = new TextureVertex[m_nrOfVerts];

	for (unsigned int i = 0; i < m_nrOfVerts; i++)
	{
		m_vertices[i] = vertices[i];
	}

	return true;
}

bool TextureModel::CreateVertexBuffer(ID3D11Device * device)
{
	//m_vertexBuffer = Buffers::CreateVertexBuffer(device, m_nrOfVerts * sizeof(SingleColorVertex), m_vertices);
	m_vertexBuffer = Buffers::CreateDynamicVertexBuffer(device, m_nrOfVerts * sizeof(TextureVertex), m_vertices);

	if (!m_vertexBuffer)
	{
		return false;
	}
	return true;
}

void TextureModel::SetupRender(ID3D11DeviceContext * deviceContext)
{
	unsigned int vertexSize = sizeof(TextureVertex);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &vertexSize, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

//Model * TextureModel::Create(ID3D11Device * device, const char * fileName)
//{
//	Model* model = new TextureModel;
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