#include "HUDObject.hpp"
#include <d3d11.h>
#include "Texture.hpp"
#include "SystemInformation.hpp"

HUDObject::HUDObject()
{
}

HUDObject::~HUDObject()
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

bool HUDObject::Initialize(ID3D11Device * device, const char * textureName, Vector2i position, Vector2i dimensions)
{
	m_dimensions = dimensions;
	m_position = Vector2i(-1, -1);

	m_nrOfVertices = 6;
	m_vertices = new Vertex[m_nrOfVertices];

	memset(m_vertices, 0, sizeof(Vertex) * m_nrOfVertices);

	m_vertices[0].uv = Vector2f(0.0f, 1.0f);	// Bottom left
	m_vertices[1].uv = Vector2f(0.0f, 0.0f);	// Upper left
	m_vertices[2].uv = Vector2f(1.0f, 0.0f);	// Upper right
	m_vertices[3].uv = Vector2f(1.0f, 0.0f);	// Upper right
	m_vertices[4].uv = Vector2f(1.0f, 1.0f);	// Bottom right
	m_vertices[5].uv = Vector2f(0.0f, 1.0f);	// Bottom left

	SetPosition(Vector2i(0, 0));

	if (!InitializeVertexBuffer(device))
	{
		return false;
	}

	m_texture = std::make_unique<Texture>();
	if (!m_texture->Initialize(device, textureName))
	{
		return false;
	}
	m_shaderResourceView = m_texture->GetShaderResourceView();

	m_vertices_changed_flag = true;

	return true;
}

bool HUDObject::InitializeVertexBuffer(ID3D11Device * device)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * m_nrOfVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = m_vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	return SUCCEEDED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer));
}

void HUDObject::SetPosition(Vector2i position)
{
	if (position == m_position)
	{
		return;
	}
	m_position = position;

	UpdateVertices();
}

void HUDObject::SetDimensions(Vector2i dimensions)
{
	if (dimensions == m_dimensions)
	{
		return;
	}
	m_dimensions = dimensions;

	UpdateVertices();
}

void HUDObject::UpdateVertices()
{
	float left, right, top, bottom, xVal, yVal;

	// Translate interval from [0, 800] to [-1, 1] 
	xVal = 2.0f / WNDW;
	yVal = 2.0f / WNDH;

	left = m_position.x * xVal - 1.0f;
	right = left + m_dimensions.x * xVal;
	top = 1.0f - m_position.y * yVal;
	bottom = top - m_dimensions.y * yVal;


	m_vertices[0].position = Vector3f(left, bottom, 0.0f);	// Bottom left
	m_vertices[1].position = Vector3f(left, top, 0.0f);		// Upper left
	m_vertices[2].position = Vector3f(right, top, 0.0f);	// Upper right
	m_vertices[3].position = Vector3f(right, top, 0.0f);	// Upper right
	m_vertices[4].position = Vector3f(right, bottom, 0.0f);	// Bottom right
	m_vertices[5].position = Vector3f(left, bottom, 0.0f);	// Bottom left

	m_vertices_changed_flag = true;
}

void HUDObject::SetupVertexBuffer(ID3D11DeviceContext * deviceContext)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	Vertex* vertexData;
	HRESULT result;

	if (!m_vertices_changed_flag)
	{
		return;
	}

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
	memcpy(vertexData, (void*)m_vertices, sizeof(Vertex) * m_nrOfVertices);

	deviceContext->Unmap(m_vertexBuffer, 0);

	m_vertices_changed_flag = false;
}

void HUDObject::SetShaderResourceView(ID3D11ShaderResourceView * shaderResourceView)
{
	m_shaderResourceView = shaderResourceView;
}

void HUDObject::SetDefaultShaderResourceView()
{
	m_shaderResourceView = GetShaderResourceView();
}

void HUDObject::Render(ID3D11DeviceContext * deviceContext)
{
	unsigned int vertexSize = sizeof(Vertex);
	unsigned int offset = 0;

	SetupVertexBuffer(deviceContext);


	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &vertexSize, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->PSSetShaderResources(0, 1, &m_shaderResourceView);

	deviceContext->Draw(m_nrOfVertices, 0);
}

ID3D11ShaderResourceView * HUDObject::GetShaderResourceView()
{
	return m_texture->GetShaderResourceView();
}