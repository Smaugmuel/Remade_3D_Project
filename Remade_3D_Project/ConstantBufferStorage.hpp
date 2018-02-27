#ifndef CONSTANT_BUFFER_STORAGE_HPP
#define CONSTANT_BUFFER_STORAGE_HPP
#include "Singleton.hpp"

#include <DirectXMath.h>
#include "Vector3.hpp"
#include "SystemInformation.hpp"

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

	ConstantBufferStorage();
	~ConstantBufferStorage();

public:
	bool Initialize(ID3D11Device* device);

	bool SetVSWorldMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX& matrix);
	bool SetVSViewMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX& matrix);
	bool SetVSProjectionMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX& matrix);
	bool SetVSPointLight(ID3D11DeviceContext* deviceContext, const Vector3f& position, float intensity);
	bool SetVSColor(ID3D11DeviceContext* deviceContext, const Vector3f& color);

	bool SetGSViewMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX& matrix);
	bool SetGSProjectionMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX& matrix);
	bool SetGSPointLight(ID3D11DeviceContext* deviceContext, const Vector3f& position, float intensity);
	bool SetGSWorldMatrixArray(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX* matrices);
	bool SetGSNrOfObjects(ID3D11DeviceContext* deviceContext, int nrOfObjects);

	bool SetPSViewMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX& matrix);
	bool SetPSProjectionMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX& matrix);
	bool SetPSPointLight(ID3D11DeviceContext* deviceContext, const Vector3f& position, float intensity);
	bool SetPSPointLightArray(ID3D11DeviceContext* deviceContext, Vector3f* positions, float* intensities);
	bool SetPSViewMatrixArray(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX* matrices);
	bool SetPSProjectionMatrixArray(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX* matrices);
	bool SetPSNrOfLights(ID3D11DeviceContext* deviceContext, int nrOfLights);

private:
	bool MapWorldMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX& matrix);
	bool MapViewMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX& matrix);
	bool MapProjectionMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX& matrix);
	bool MapPointLight(ID3D11DeviceContext * deviceContext, const Vector3f& position, float intensity);
	bool MapPointLightArray(ID3D11DeviceContext * deviceContext, Vector3f* positions, float* intensities);
	bool MapColor(ID3D11DeviceContext * deviceContext, const Vector3f& color);
	bool MapViewMatrixArray(ID3D11DeviceContext * deviceContext, DirectX::XMMATRIX* matrices);
	bool MapProjectionMatrixArray(ID3D11DeviceContext * deviceContext, DirectX::XMMATRIX* matrices);
	bool MapNrOfLights(ID3D11DeviceContext * deviceContext, int nrOfLights);
	bool MapWorldMatrixArray(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX* matrices);
	bool MapNrOfObjects(ID3D11DeviceContext* deviceContext, int nrOfObjects);

	ID3D11Buffer* m_worldBuffer;
	ID3D11Buffer* m_viewBuffer;
	ID3D11Buffer* m_projectionBuffer;
	ID3D11Buffer* m_pointLightBuffer;
	ID3D11Buffer* m_pointLightArrayBuffer;
	ID3D11Buffer* m_colorBuffer;
	ID3D11Buffer* m_viewArrayBuffer;
	ID3D11Buffer* m_projectionArrayBuffer;
	ID3D11Buffer* m_nrOfLightsBuffer;
	ID3D11Buffer* m_worldArrayBuffer;
	ID3D11Buffer* m_nrOfObjectsBuffer;
};

#endif