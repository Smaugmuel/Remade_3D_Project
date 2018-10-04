#include "ConstantBufferManager.hpp"
#include <d3d11.h>

ConstantBufferManager::ConstantBufferManager() : m_device(nullptr), m_deviceContext(nullptr)
{

}
ConstantBufferManager::~ConstantBufferManager()
{
	for (unsigned int i = 0; i < MAX_NR_OF_BUFFERS/*m_buffers.size()*/; i++)
	{
		if (m_buffers[i].buffer)
		{
			m_buffers[i].buffer->Release();
		}
	}
}

bool ConstantBufferManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	m_device = device;
	m_deviceContext = deviceContext;
	m_nrOfBuffers = 0;
	return true;
}

int ConstantBufferManager::CreateConstantBuffer(unsigned int bufferSize, void* initialData)
{
	/*
	Simple "solution" for now
	*/
	if (m_nrOfBuffers == MAX_NR_OF_BUFFERS)
	{
		return -1;
	}

	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;
	Buffer buffer;

	memset(&desc, 0, sizeof(desc));
	desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = buffer.bufferSize = bufferSize;
	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	data.pSysMem = initialData;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;
	
	HRESULT result = m_device->CreateBuffer(&desc, initialData ? &data : nullptr, &buffer.buffer);
	if (FAILED(result))
		return -1;
	
	m_buffers[m_nrOfBuffers] = buffer;
	return m_nrOfBuffers++;
	/*m_buffers.push_back(buffer);
	return m_buffers.size() - 1;*/
}

bool ConstantBufferManager::MapDataToBuffer(int index, void * bufferData)
{
	if (index < 0 || index >= static_cast<int>(m_nrOfBuffers/*m_buffers.size()*/))
		return false;

	const int x = D3D11_PS_CS_UAV_REGISTER_COUNT;

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	if (FAILED(m_deviceContext->Map(m_buffers[index].buffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	memcpy(mappedResource.pData, bufferData, m_buffers[index].bufferSize);

	m_deviceContext->Unmap(m_buffers[index].buffer, 0);

	return true;
}

bool ConstantBufferManager::SetConstantBufferToVertexShader(int index, int slotInShader)
{
	if (index < 0 || index >= static_cast<int>(m_nrOfBuffers/*m_nbuffers.size()*/))
		return false;

	m_deviceContext->VSSetConstantBuffers(slotInShader, 1, &m_buffers[index].buffer);
	return true;
}
bool ConstantBufferManager::SetConstantBufferToGeometryShader(int index, int slotInShader)
{
	if (index < 0 || index >= static_cast<int>(m_nrOfBuffers/*m_nbuffers.size()*/))
		return false;

	m_deviceContext->GSSetConstantBuffers(slotInShader, 1, &m_buffers[index].buffer);
	return true;
}
bool ConstantBufferManager::SetConstantBufferToPixelShader(int index, int slotInShader)
{
	if (index < 0 || index >= static_cast<int>(m_nrOfBuffers/*m_nbuffers.size()*/))
		return false;

	m_deviceContext->PSSetConstantBuffers(slotInShader, 1, &m_buffers[index].buffer);
	return true;
}