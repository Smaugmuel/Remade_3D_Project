#include "ConstantBufferManager.hpp"
#include <d3d11.h>

ConstantBufferManager::ConstantBufferManager() : m_device(nullptr), m_deviceContext(nullptr)
{

}
ConstantBufferManager::~ConstantBufferManager()
{
	for (unsigned int i = 0; i < m_buffers.size(); i++)
	{
		m_buffers[i].buffer->Release();
	}
}

bool ConstantBufferManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	m_device = device;
	m_deviceContext = deviceContext;
	return true;
}

int ConstantBufferManager::CreateConstantBuffer(unsigned int bufferSize, void* initialData)
{
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
	
	HRESULT result = m_device->CreateBuffer(&desc, nullptr/*&data*/, &buffer.buffer);
	if (FAILED(result))
		return -1;
	
	m_buffers.push_back(buffer);
	return m_buffers.size() - 1;
}

bool ConstantBufferManager::MapDataToBuffer(int index, void * bufferData)
{
	if (index < 0 || index >= static_cast<int>(m_buffers.size()))
		return false;

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	if (FAILED(m_deviceContext->Map(m_buffers[index].buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	memcpy(mappedResource.pData, bufferData, m_buffers[index].bufferSize);

	m_deviceContext->Unmap(m_buffers[index].buffer, 0);

	return true;
}

bool ConstantBufferManager::SetConstantBufferToVertexShader(int index, int slotInShader)
{
	if (index < 0 || index >= static_cast<int>(m_buffers.size()))
		return false;

	m_deviceContext->VSSetConstantBuffers(slotInShader, 1, &m_buffers[index].buffer);
	return true;
}
bool ConstantBufferManager::SetConstantBufferToGeometryShader(int index, int slotInShader)
{
	if (index < 0 || index >= static_cast<int>(m_buffers.size()))
		return false;

	m_deviceContext->GSSetConstantBuffers(slotInShader, 1, &m_buffers[index].buffer);
	return true;
}
bool ConstantBufferManager::SetConstantBufferToPixelShader(int index, int slotInShader)
{
	if (index < 0 || index >= static_cast<int>(m_buffers.size()))
		return false;

	m_deviceContext->PSSetConstantBuffers(slotInShader, 1, &m_buffers[index].buffer);
	return true;
}