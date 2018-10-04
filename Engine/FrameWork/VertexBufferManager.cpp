#include "VertexBufferManager.hpp"
#include <d3d11.h>

VertexBufferManager::VertexBufferManager() : m_device(nullptr), m_deviceContext(nullptr), m_nrOfBuffers(0)
{
}

VertexBufferManager::~VertexBufferManager()
{
	for (unsigned int i = 0; i < m_nrOfBuffers; i++)
	{
		if (m_buffers[i])
		{
			m_buffers[i]->Release();
		}
	}
}

bool VertexBufferManager::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext)
{
	m_device = device;
	m_deviceContext = deviceContext;
	return true;
}

int VertexBufferManager::CreateBuffer(unsigned int bufferSize, unsigned int vertexSize, void * initialData)
{
	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;

	if (m_nrOfBuffers == MAX_NR_OF_VERTEX_BUFFERS)
	{
		return -1;
	}

	memset(&desc, 0, sizeof(desc));
	desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	desc.ByteWidth = bufferSize;
	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	data.pSysMem = initialData;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	if (FAILED(m_device->CreateBuffer(&desc, &data, &m_buffers[m_nrOfBuffers])))
		return -1;

	m_vertexSizes[m_nrOfBuffers] = vertexSize;

	return m_nrOfBuffers++;
}

bool VertexBufferManager::SetBufferToInputAssembler(int index, int slotInShader)
{
	if (index < 0 || index >= static_cast<int>(m_nrOfBuffers))
		return false;

	unsigned int offset = 0;
	
	m_deviceContext->IASetVertexBuffers(slotInShader, 1, &m_buffers[index], &m_vertexSizes[index], &offset);
	
	return true;
}