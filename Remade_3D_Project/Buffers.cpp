#include "Buffers.hpp"
#include <d3d11.h>

ID3D11Buffer* Buffers::CreateConstantBuffer(ID3D11Device* device, const unsigned int& bufferSize)
{
	D3D11_BUFFER_DESC description;
	ID3D11Buffer* buffer;

	memset(&description, 0, sizeof(description));
	description.Usage = D3D11_USAGE_DYNAMIC;
	description.ByteWidth = bufferSize;
	description.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	description.MiscFlags = 0;
	description.StructureByteStride = 0;

	if (FAILED(device->CreateBuffer(&description, nullptr, &buffer)))
	{
		return nullptr;
	}
	return buffer;
}

ID3D11Buffer* Buffers::CreateVertexBuffer(ID3D11Device* device, const unsigned int& bufferSize, void* initialData)
{
	D3D11_BUFFER_DESC description;
	D3D11_SUBRESOURCE_DATA data;
	ID3D11Buffer* buffer;

	memset(&description, 0, sizeof(description));
	description.Usage = D3D11_USAGE_DEFAULT;
	description.ByteWidth = bufferSize;
	description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	description.CPUAccessFlags = 0;
	description.MiscFlags = 0;
	description.StructureByteStride = 0;

	data.pSysMem = initialData;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&description, &data, &buffer)))
	{
		return nullptr;
	}
	return buffer;
}
