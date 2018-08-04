#include "DeferredScreenTarget.hpp"
#include <d3d11.h>

DeferredScreenTarget* Singleton<DeferredScreenTarget>::s_instance = nullptr;

DeferredScreenTarget::DeferredScreenTarget()
{
}

DeferredScreenTarget::~DeferredScreenTarget()
{
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}
}

bool DeferredScreenTarget::Initialize(ID3D11Device* device)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;

	/* Use an entire quad */
	//m_vertices[0].position = Vector3f(-1.0f, -1.0f, 0.0f);
	//m_vertices[1].position = Vector3f(-1.0f, 1.0f, 0.0f);
	//m_vertices[2].position = Vector3f(1.0f, 1.0f, 0.0f);
	//m_vertices[3].position = Vector3f(1.0f, 1.0f, 0.0f);
	//m_vertices[4].position = Vector3f(1.0f, -1.0f, 0.0f);
	//m_vertices[5].position = Vector3f(-1.0f, -1.0f, 0.0f);
	//m_vertices[0].uv = Vector2f(0.0f, 1.0f);
	//m_vertices[1].uv = Vector2f(0.0f, 0.0f);
	//m_vertices[2].uv = Vector2f(1.0f, 0.0f);
	//m_vertices[3].uv = Vector2f(1.0f, 0.0f);
	//m_vertices[4].uv = Vector2f(1.0f, 1.0f);
	//m_vertices[5].uv = Vector2f(0.0f, 1.0f);

	/* Use only a triangle and not a quad */
	m_vertices[0].position = Vector3f(-3.0f, -1.0f, 0.0f);	// bottom left
	m_vertices[1].position = Vector3f(1.0f, 3.0f, 0.0f);	// upper right
	m_vertices[2].position = Vector3f(1.0f, -1.0f, 0.0f);	// bottom right
	m_vertices[0].uv = Vector2f(-1.0f, 1.0f);
	m_vertices[1].uv = Vector2f(1.0f, -1.0f);
	m_vertices[2].uv = Vector2f(1.0f, 1.0f);

	/* Cut screen in half */
	//m_vertices[0].position = Vector3f(-1.0f, -1.0f, 0.0f);	// bottom left
	//m_vertices[1].position = Vector3f(1.0f, 1.0f, 0.0f);	// upper right
	//m_vertices[2].position = Vector3f(1.0f, -1.0f, 0.0f);	// bottom right
	//m_vertices[0].uv = Vector2f(0.0f, 1.0f);
	//m_vertices[1].uv = Vector2f(1.0f, 0.0f);
	//m_vertices[2].uv = Vector2f(1.0f, 1.0f);

	// Create vertex buffer ===============================================================
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(m_vertices);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = m_vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		return false;
	}

	return true;
}

void DeferredScreenTarget::Render(ID3D11DeviceContext * deviceContext)
{
	unsigned int vertexSize = sizeof(Vertex);
	unsigned int offset = 0;
	unsigned int nrOfVertices = sizeof(m_vertices) / vertexSize;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &vertexSize, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->Draw(nrOfVertices, 0);
}