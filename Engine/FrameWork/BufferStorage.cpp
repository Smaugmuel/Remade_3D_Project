#include "BufferStorage.hpp"
#include "Direct3D.hpp"

BufferStorage::BufferStorage()
{

}
BufferStorage::~BufferStorage()
{
	for (unsigned int i = 0; i < m_vertexBuffers.size(); i++)
	{
		m_vertexBuffers[i]->Release();
	}

	for (unsigned int i = 0; i < m_constantBuffers.size(); i++)
	{
		m_constantBuffers[i]->Release();
	}
}

int BufferStorage::CreateVertexBuffer(unsigned int bufferSize, void* initialData)
{
	D3D11_BUFFER_DESC description;
	D3D11_SUBRESOURCE_DATA data;
	ID3D11Buffer* buffer;

	memset(&description, 0, sizeof(description));
	description.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	description.ByteWidth = bufferSize;
	description.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	description.CPUAccessFlags = 0;
	description.MiscFlags = 0;
	description.StructureByteStride = 0;

	data.pSysMem = initialData;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	if (FAILED(Direct3D::Get()->GetDevice()->CreateBuffer(&description, &data, &buffer)))
	{
		buffer->Release();
		return -1;
	}
	
	m_vertexBuffers.push_back(buffer);
	return m_vertexBuffers.size() - 1;
}

int BufferStorage::CreateConstantBuffer(unsigned int bufferSize, void* initialData)
{
	D3D11_BUFFER_DESC description;
	D3D11_SUBRESOURCE_DATA data;
	ID3D11Buffer* buffer;

	memset(&description, 0, sizeof(description));
	description.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	description.ByteWidth = bufferSize;
	description.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	description.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	description.MiscFlags = 0;
	description.StructureByteStride = 0;

	data.pSysMem = initialData;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	if (FAILED(Direct3D::Get()->GetDevice()->CreateBuffer(&description, &data, &buffer)))
	{
		buffer->Release();
		return -1;
	}
	
	m_constantBuffers.push_back(buffer);
	return m_constantBuffers.size() - 1;
}