#include "BitMap.hpp"
#include <d3d11.h>

BitMap::BitMap()
{
}

BitMap::~BitMap()
{
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}
	if (m_vertices)
	{
		delete[] m_vertices;
		m_vertices = nullptr;
	}
}

bool BitMap::Initialize(ID3D11Device * device, Vector2i screenDimensions, Vector2i bitMapDimensions)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	unsigned int vertexBufferSize;

	m_screenDimensions = screenDimensions;
	m_bitMapDimensions = bitMapDimensions;
	m_previousPosition = Vector2i(-1, -1);


	m_nrOfVertices = 6;
	m_vertices = new Vertex[m_nrOfVertices];

	m_vertices[0].uv = Vector2f(0.0f, 1.0f);	// Bottom left
	m_vertices[1].uv = Vector2f(0.0f, 0.0f);	// Upper left
	m_vertices[2].uv = Vector2f(1.0f, 0.0f);	// Upper right
	m_vertices[3].uv = Vector2f(1.0f, 0.0f);	// Upper right
	m_vertices[4].uv = Vector2f(1.0f, 1.0f);	// Bottom right
	m_vertices[5].uv = Vector2f(0.0f, 1.0f);	// Bottom left

	SetPosition(Vector2i(0, 0));


	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * m_nrOfVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
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

void BitMap::SetPosition(Vector2i position)
{
	float left, right, top, bottom;

	if (position == m_previousPosition)
	{
		return;
	}

	m_previousPosition = position;

	left = (float)(-m_screenDimensions.x / 2.0f) + (float)position.x;
	right = left + (float)m_bitMapDimensions.x;
	top = (float)(m_screenDimensions.y / 2.0f) - (float)position.y;
	bottom = top - (float)m_bitMapDimensions.y;

	m_vertices[0].position = Vector3f(left, bottom, 0.0f);	// Bottom left
	m_vertices[1].position = Vector3f(left, top, 0.0f);		// Upper left
	m_vertices[2].position = Vector3f(right, top, 0.0f);	// Upper right
	m_vertices[3].position = Vector3f(right, top, 0.0f);	// Upper right
	m_vertices[4].position = Vector3f(right, bottom, 0.0f);	// Bottom right
	m_vertices[5].position = Vector3f(left, bottom, 0.0f);	// Bottom left
}

void BitMap::SetupVertexBuffer(ID3D11DeviceContext * deviceContext)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	Vertex* vertexData;
	HRESULT result;

	result = deviceContext->Map(
		m_vertexBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource
		);
	if (FAILED(result))
	{
		return;
	}

	vertexData = (Vertex*)mappedResource.pData;
	for (unsigned int i = 0; i < m_nrOfVertices; i++)
	{
		vertexData[i] = m_vertices[i];
	}

	deviceContext->Unmap(m_vertexBuffer, 0);
}

void BitMap::SetShaderResourceView(ID3D11ShaderResourceView * shaderResourceView)
{
	m_texture = shaderResourceView;
}

void BitMap::Render(ID3D11DeviceContext * deviceContext)
{
	unsigned int vertexSize = sizeof(Vertex);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &vertexSize, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//deviceContext->PSS

	deviceContext->Draw(m_nrOfVertices, 0);
}

ID3D11ShaderResourceView * BitMap::GetShaderResourceView()
{
	return m_texture;
}