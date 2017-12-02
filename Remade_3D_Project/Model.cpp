#include "Model.hpp"
#include <d3d11.h>

//#include <sstream>
//#include <fstream>
//
//#include "Vector3.hpp"
//#include "Vector2.hpp"
//
//#include <vector>
//
//Model::Model()
//{
//}
//
//Model::~Model()
//{
//}
//
//bool Model::LoadFromFile(const std::string& fileName)
//{
//	std::ifstream infile(fileName);
//	std::string line, type;
//	std::vector<Vector3f> vertices;
//	std::vector<Vector3f> normals;
//	std::vector<Vector2f> UVs;
//	float x, y, z;
//
//
//	while (std::getline(infile, line))
//	{
//		type = "";
//		std::istringstream iss(line);
//
//		iss >> type;
//
//		if (type == "v")
//		{
//			iss >> x >> y >> z;
//			vertices.push_back(Vector3f(x, y, z));
//		}
//		else if (type == "vn")
//		{
//			iss >> x >> y >> z;
//			normals.push_back(Vector3f(x, y, z));
//		}
//		else if (type == "vt")
//		{
//			iss >> x >> y;
//			UVs.push_back(Vector2f(x, y));
//		}
//		else if (type == "f")
//		{
//			//m_nrOfFaces++;
//			int vi[4];
//			int ni[4];
//			int uvi[4];
//
//			int n = 0;
//
//			std::string s;
//			while (iss >> s)
//			{
//				s.replace(s.find("/"), 1, " ");
//				s.replace(s.find("/"), 1, " ");
//
//				std::istringstream iss(s);
//
//				if (m_textureState == STATE_COLOR)
//					iss >> vi[n] >> ni[n];
//				else
//					iss >> vi[n] >> uvi[n] >> ni[n];
//
//				vi[n] = vi[n] - 1;
//				uvi[n] = uvi[n] - 1;
//				ni[n] = ni[n++] - 1;
//
//				if (m_textureState == STATE_COLOR)
//					/*Log*/ *m_outfile << " V" << vi[n - 1] << " N" << ni[n - 1];
//				else
//					/*Log*/ *m_outfile << " V" << vi[n - 1] << " UV" << uvi[n - 1] << " N" << ni[n - 1];
//			}
//			*m_outfile << "\n";
//
//
//			if (m_textureState == STATE_NORMAL_MAP || m_textureState == STATE_TEXTURE)
//			{
//				Vertex_Texture v;
//
//				// Triangle 1
//				v.pos = vertexPositions.at(vi[0]);
//				v.uv = vertexUVs.at(uvi[0]);
//				v.normal = vertexNormals.at(ni[0]);
//				m_vertexTextureData.push_back(v);
//
//				v.pos = vertexPositions.at(vi[1]);
//				v.uv = vertexUVs.at(uvi[1]);
//				v.normal = vertexNormals.at(ni[1]);
//				m_vertexTextureData.push_back(v);
//
//				v.pos = vertexPositions.at(vi[2]);
//				v.uv = vertexUVs.at(uvi[2]);
//				v.normal = vertexNormals.at(ni[2]);
//				m_vertexTextureData.push_back(v);
//
//
//				// Triangle 2
//				v.pos = vertexPositions.at(vi[2]);
//				v.uv = vertexUVs.at(uvi[2]);
//				v.normal = vertexNormals.at(ni[2]);
//				m_vertexTextureData.push_back(v);
//
//				v.pos = vertexPositions.at(vi[3]);
//				v.uv = vertexUVs.at(uvi[3]);
//				v.normal = vertexNormals.at(ni[3]);
//				m_vertexTextureData.push_back(v);
//
//				v.pos = vertexPositions.at(vi[0]);
//				v.uv = vertexUVs.at(uvi[0]);
//				v.normal = vertexNormals.at(ni[0]);
//				m_vertexTextureData.push_back(v);
//			}
//			else
//			{
//				Vertex_Color v;
//
//				// Triangle 1
//				v.pos = vertexPositions.at(vi[0]);
//				v.color = m_material->GetAmbient();		// Same color for all vertices below
//				v.normal = vertexNormals.at(ni[0]);
//				m_vertexColorData.push_back(v);
//
//				v.pos = vertexPositions.at(vi[1]);
//				v.normal = vertexNormals.at(ni[1]);
//				m_vertexColorData.push_back(v);
//
//				v.pos = vertexPositions.at(vi[2]);
//				v.normal = vertexNormals.at(ni[2]);
//				m_vertexColorData.push_back(v);
//
//
//				// Triangle 2
//				v.pos = vertexPositions.at(vi[2]);
//				v.normal = vertexNormals.at(ni[2]);
//				m_vertexColorData.push_back(v);
//
//				v.pos = vertexPositions.at(vi[3]);
//				v.normal = vertexNormals.at(ni[3]);
//				m_vertexColorData.push_back(v);
//
//				v.pos = vertexPositions.at(vi[0]);
//				v.normal = vertexNormals.at(ni[0]);
//				m_vertexColorData.push_back(v);
//			}
//		}
//		else if (type == "mtllib")
//		{
//			iss >> line;
//			if (!m_material->LoadMaterial(device, line))
//				return false;
//
//			m_textureState = (TextureState)m_material->GetNrOfResources();
//		}
//		else
//		{
//			/*Log*/ *m_outfile << "Line Read ERROR: " << line << "\n";
//		}
//	}
//
//	*m_outfile << "\n===Model Info===\n";
//	*m_outfile << "Vertices Found: " << vertexPositions.size() << "\n";
//	*m_outfile << "Normals Found: " << vertexNormals.size() << "\n";
//	*m_outfile << "Faces Found: " << m_nrOfFaces << "\n";
//	*m_outfile << "Triangles Made: " << (m_vertexColorData.size() + m_vertexTextureData.size()) / 3 << "\n";		// Only one vector has more than 0
//
//
//	unsigned int nrOfVertices = 0;
//	unsigned int nrOfNormals = 0;
//	unsigned int nrOfFaces = 0;
//	unsigned int nrOfUVs = 0;
//
//	GetAmounts(nrOfVertices, nrOfNormals, nrOfFaces, nrOfUVs);
//
//	return false;
//}
//
//bool Model::CreateVertexBuffer(ID3D11Device * device)
//{
//	return false;
//}
//
//void Model::GetAmounts(unsigned int& nrOfVertices, unsigned int& nrOfNormals, unsigned int& nrOfFaces, unsigned int& nrOfUVs)
//{
//	
//}

Model::Model()
{
}

Model::~Model()
{
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}
}