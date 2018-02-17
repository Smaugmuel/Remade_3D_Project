#ifndef CONSTANT_BUFFER_STORAGE_HPP
#define CONSTANT_BUFFER_STORAGE_HPP
#include "Singleton.hpp"

//#include <string>
//#include <map>
//#include <d3d11.h>
//#include "ConstantBufferTypes.hpp"

#include <DirectXMath.h>
#include "Vector3.hpp"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Buffer;

/*


WARNING!

For some reason, this object will cause program to crash while loading models, or simply say "wnb not loaded"

Sometimes it works, but avoid using this until further proof of it working


*/

//template<typename BufferData>
//struct ConstantBuffer
//{
//	ConstantBuffer()
//	{
//		m_buffer = nullptr;
//	}
//	~ConstantBuffer()
//	{
//
//	}
//
//	bool Initialize(ID3D11Device* device)
//	{
//		D3D11_BUFFER_DESC desc;
//
//		memset(&m_buffer, 0, sizeof(BufferData));
//		desc.Usage = D3D11_USAGE_DYNAMIC;
//		desc.ByteWidth = sizeof(BufferData);
//		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//		desc.MiscFlags = 0;
//		desc.StructureByteStride = 0;
//
//		return SUCCEEDED(device->CreateBuffer(&desc, nullptr, &m_buffer));
//	}
//	bool MapData(ID3D11DeviceContext* deviceContext)
//	{
//		D3D11_MAPPED_SUBRESOURCE mappedResource;
//		BufferData* dataPtr;
//		HRESULT result;
//
//		result = deviceContext->Map(
//			m_buffer,
//			0,
//			D3D11_MAP_WRITE_DISCARD,
//			0,
//			&mappedResource
//		);
//		if (FAILED(result))
//		{
//			return false;
//		}
//
//		dataPtr = static_cast<BufferData*>(mappedResource.pData);
//		*dataPtr = m_data;
//
//		deviceContext->Unmap(m_buffer, 0);
//
//		return true;
//	}
//
//	BufferData m_data;
//	ID3D11Buffer* m_buffer;
//};


class ConstantBufferStorage : public Singleton<ConstantBufferStorage>
{
	friend class Singleton<ConstantBufferStorage>;

	/*typedef std::map<std::string, ID3D11Buffer*> Buffers;
	typedef std::map<std::string, unsigned int> BufferSizes;*/

	ConstantBufferStorage();
	~ConstantBufferStorage();

public:
	bool Initialize(ID3D11Device* device);

	bool SetWorldMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX& matrix);
	bool SetViewMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX& matrix);
	bool SetProjectionMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX& matrix);
	bool SetPointLight(ID3D11DeviceContext* deviceContext, const Vector3f& position, float intensity);
	bool SetColor(ID3D11DeviceContext* deviceContext, const Vector3f& color);

private:
	ID3D11Buffer* m_worldBuffer;
	ID3D11Buffer* m_viewBuffer;
	ID3D11Buffer* m_projectionBuffer;
	ID3D11Buffer* m_pointLightBuffer;
	ID3D11Buffer* m_colorBuffer;

	/*Buffers m_buffers;
	BufferSizes m_bufferSizes;*/
};

#endif