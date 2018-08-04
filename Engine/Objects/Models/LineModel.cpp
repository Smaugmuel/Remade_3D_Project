#include "LineModel.hpp"

#include "../../Buffers/Buffers.hpp"
#include "../../FrameWork/Direct3D.hpp"

LineModel::LineModel() : Model()
{
	m_nrOfVerts = 2;

	m_vertices[0].position.x = -0.5f;
	m_vertices[1].position.x = 0.5f;

	// Default cyan, although this is unused
	m_vertices[0].color = m_vertices[1].color = Vector3f(0, 1, 1);
}

LineModel::~LineModel()
{
}

bool LineModel::LoadFromFile(const char * fileName)
{
	return true;
}

bool LineModel::CreateVertexBuffer()
{
	m_vertexBuffer = Buffers::CreateVertexBuffer(Direct3D::Get()->GetDevice(), m_nrOfVerts * sizeof(LineVertex), m_vertices);

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

void LineModel::SetupRender()
{
	ID3D11DeviceContext * deviceContext = Direct3D::Get()->GetDeviceContext();
	unsigned int vertexSize = sizeof(LineVertex);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &vertexSize, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}