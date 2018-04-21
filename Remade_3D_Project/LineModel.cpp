#include "LineModel.hpp"

#include "Buffers.hpp"
#include <d3d11.h>

LineModel::LineModel() : Model()
{
	m_nrOfVerts = 2;

	m_vertices[0].position.x = -0.5f;
	m_vertices[1].position.x = 0.5f;

	m_vertices[0].color.y = m_vertices[1].color.y = 1.0f;
	m_vertices[0].color.z = m_vertices[1].color.z = 1.0f;
}

LineModel::~LineModel()
{
}

bool LineModel::LoadFromFile(const char * fileName)
{
	return true;
}

bool LineModel::CreateVertexBuffer(ID3D11Device * device)
{
	m_vertexBuffer = Buffers::CreateVertexBuffer(device, m_nrOfVerts * sizeof(LineVertex), m_vertices);

	if (!m_vertexBuffer)
	{
		return false;
	}
	return true;
}

bool LineModel::CreateOBB()
{
	return false;
}

void LineModel::SetupRender(ID3D11DeviceContext * deviceContext)
{
	unsigned int vertexSize = sizeof(LineVertex);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &vertexSize, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}