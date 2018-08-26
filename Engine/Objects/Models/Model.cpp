#include "Model.hpp"
//#include "../../FrameWork/Direct3D.hpp"
//#include "../../Core/Engine.hpp"
#include "../../FrameWork/FrameWork.hpp"

Model::Model(FrameWork* frameWork) :
	m_vertexBuffer(nullptr),
	m_nrOfVerts(0),
	m_frameWork(frameWork)
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
	m_frameWork->GetDirect3D()->GetDeviceContext()->Draw(m_nrOfVerts, 0);
	//Direct3D::Get()->GetDeviceContext()->Draw(m_nrOfVerts, 0);
}

const OBB& Model::GetOBB() const
{
	return m_obb;
}
