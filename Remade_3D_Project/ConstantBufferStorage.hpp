#ifndef CONSTANT_BUFFER_STORAGE_HPP
#define CONSTANT_BUFFER_STORAGE_HPP
#include "Singleton.hpp"
#include <string>
#include <map>

#include <d3d11.h>



/*


WARNING!

For some reason, this object will cause program to crash while loading models, or simply say "wnb not loaded"

Sometimes it works, but avoid using this until further proof of it working


*/



template<typename BufferData>
struct ConstantBuffer
{
	ConstantBuffer()
	{
		m_buffer = nullptr;
	}
	~ConstantBuffer()
	{

	}

	bool Initialize(ID3D11Device* device)
	{
		D3D11_BUFFER_DESC desc;

		memset(&m_buffer, 0, sizeof(BufferData));
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = sizeof(BufferData);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		return SUCCEEDED(device->CreateBuffer(&desc, nullptr, &m_buffer));
	}
	bool MapData(ID3D11DeviceContext* deviceContext)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		BufferData* dataPtr;
		HRESULT result;

		result = deviceContext->Map(
			m_buffer,
			0,
			D3D11_MAP_WRITE_DISCARD,
			0,
			&mappedResource
		);
		if (FAILED(result))
		{
			return false;
		}

		dataPtr = static_cast<BufferData*>(mappedResource.pData);
		*dataPtr = m_data;

		deviceContext->Unmap(m_buffer, 0);

		return true;
	}

	BufferData m_data;
	ID3D11Buffer* m_buffer;
};


/*class ConstantBufferStorage : public Singleton<ConstantBufferStorage>
{
	friend class Singleton<ConstantBufferStorage>;

	typedef std::map<std::string, ID3D11Buffer*> Buffers;
	typedef std::map<std::string, unsigned int> BufferSizes;

	ConstantBufferStorage();
	~ConstantBufferStorage();

public:


private:
	Buffers m_buffers;
	BufferSizes m_bufferSizes;
};*/

#endif