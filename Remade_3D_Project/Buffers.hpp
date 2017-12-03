#ifndef BUFFERS_HPP
#define BUFFERS_HPP

struct ID3D11Device;
struct ID3D11Buffer;

namespace Buffers
{
	ID3D11Buffer* CreateConstantBuffer(ID3D11Device* device, const unsigned int& bufferSize);
	ID3D11Buffer* CreateVertexBuffer(ID3D11Device* device, const unsigned int& bufferSize, void* initialData);
	ID3D11Buffer* CreateDynamicVertexBuffer(ID3D11Device* device, const unsigned int& bufferSize, void* initialData);
}

#endif