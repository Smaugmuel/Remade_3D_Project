#include "Model.hpp"
#include "../../FrameWork/Direct3D.hpp"

Model::Model() :
	m_vertexBuffer(nullptr),
	m_nrOfVerts(0)
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

void Model::Render()
{
	Direct3D::Get()->GetDeviceContext()->Draw(m_nrOfVerts, 0);
}

const OBB& Model::GetOBB() const
{
	return m_obb;
}
