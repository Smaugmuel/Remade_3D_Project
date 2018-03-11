#include "ConstantBufferStorage.hpp"

#include <d3d11.h>
#include "ConstantBufferTypes.hpp"

ConstantBufferStorage* Singleton<ConstantBufferStorage>::s_instance = nullptr;

ConstantBufferStorage::ConstantBufferStorage()
{
}
ConstantBufferStorage::~ConstantBufferStorage()
{
}

bool ConstantBufferStorage::Initialize(ID3D11Device * device)
{
	D3D11_BUFFER_DESC desc;

	memset(&desc, 0, sizeof(desc));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	desc.ByteWidth = sizeof(Matrix);
	if (FAILED(device->CreateBuffer(&desc, nullptr, &m_worldBuffer)))
		return false;

	desc.ByteWidth = sizeof(Matrix);
	if (FAILED(device->CreateBuffer(&desc, nullptr, &m_viewBuffer)))
		return false;

	desc.ByteWidth = sizeof(Matrix);
	if (FAILED(device->CreateBuffer(&desc, nullptr, &m_projectionBuffer)))
		return false;

	desc.ByteWidth = sizeof(Float4);
	if (FAILED(device->CreateBuffer(&desc, nullptr, &m_pointLightBuffer)))
		return false;

	desc.ByteWidth = sizeof(Float4Array);
	if (FAILED(device->CreateBuffer(&desc, nullptr, &m_pointLightArrayBuffer)))
		return false;

	desc.ByteWidth = sizeof(Float4);
	if (FAILED(device->CreateBuffer(&desc, nullptr, &m_colorBuffer)))
		return false;

	desc.ByteWidth = sizeof(MatrixArray);
	if (FAILED(device->CreateBuffer(&desc, nullptr, &m_viewArrayBuffer)))
		return false;

	desc.ByteWidth = sizeof(MatrixArray);
	if (FAILED(device->CreateBuffer(&desc, nullptr, &m_projectionArrayBuffer)))
		return false;

	desc.ByteWidth = sizeof(Int4);
	if (FAILED(device->CreateBuffer(&desc, nullptr, &m_nrOfLightsBuffer)))
		return false;

	desc.ByteWidth = sizeof(MatrixChunk);
	if (FAILED(device->CreateBuffer(&desc, nullptr, &m_worldArrayBuffer)))
		return false;

	desc.ByteWidth = sizeof(Int4);
	if (FAILED(device->CreateBuffer(&desc, nullptr, &m_nrOfObjectsBuffer)))
		return false;

	return true;
}

bool ConstantBufferStorage::SetVSWorldMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & matrix)
{
	if (!MapWorldMatrix(deviceContext, matrix))
		return false;

	deviceContext->VSSetConstantBuffers(0, 1, &m_worldBuffer);

	return true;
}
bool ConstantBufferStorage::SetVSViewMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & matrix)
{
	if (!MapViewMatrix(deviceContext, matrix))
		return false;

	deviceContext->VSSetConstantBuffers(1, 1, &m_viewBuffer);

	return true;
}
bool ConstantBufferStorage::SetVSProjectionMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & matrix)
{
	if (!MapProjectionMatrix(deviceContext, matrix))
		return false;

	deviceContext->VSSetConstantBuffers(2, 1, &m_projectionBuffer);

	return true;
}
bool ConstantBufferStorage::SetVSPointLight(ID3D11DeviceContext * deviceContext, const Vector3f & position, float intensity)
{
	if (!MapPointLight(deviceContext, position, intensity))
		return false;

	deviceContext->VSSetConstantBuffers(3, 1, &m_pointLightBuffer);

	return true;
}
bool ConstantBufferStorage::SetVSColor(ID3D11DeviceContext * deviceContext, const Vector3f & color)
{
	if (!MapColor(deviceContext, color))
		return false;

	deviceContext->VSSetConstantBuffers(4, 1, &m_colorBuffer);

	return true;
}

bool ConstantBufferStorage::SetGSViewMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & matrix)
{
	if (!MapViewMatrix(deviceContext, matrix))
		return false;

	deviceContext->GSSetConstantBuffers(1, 1, &m_viewBuffer);

	return true;
}
bool ConstantBufferStorage::SetGSProjectionMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & matrix)
{
	if (!MapProjectionMatrix(deviceContext, matrix))
		return false;

	deviceContext->GSSetConstantBuffers(2, 1, &m_projectionBuffer);

	return true;
}
bool ConstantBufferStorage::SetGSWorldMatrixArray(ID3D11DeviceContext * deviceContext, DirectX::XMMATRIX * matrices)
{
	if (!MapWorldMatrixArray(deviceContext, matrices))
		return false;

	deviceContext->GSSetConstantBuffers(3, 1, &m_worldArrayBuffer);

	return true;
}
bool ConstantBufferStorage::SetGSNrOfObjects(ID3D11DeviceContext * deviceContext, int nrOfObjects)
{
	if (!MapNrOfObjects(deviceContext, nrOfObjects))
		return false;

	deviceContext->GSSetConstantBuffers(4, 1, &m_nrOfObjectsBuffer);

	return true;
}

bool ConstantBufferStorage::SetPSViewMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & matrix)
{
	if (!MapViewMatrix(deviceContext, matrix))
		return false;

	deviceContext->PSSetConstantBuffers(0, 1, &m_viewBuffer);

	return true;
}
bool ConstantBufferStorage::SetPSProjectionMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & matrix)
{
	if (!MapProjectionMatrix(deviceContext, matrix))
		return false;

	deviceContext->PSSetConstantBuffers(1, 1, &m_projectionBuffer);

	return true;
}
bool ConstantBufferStorage::SetPSViewMatrixArray(ID3D11DeviceContext * deviceContext, DirectX::XMMATRIX * matrices)
{
	if (!MapViewMatrixArray(deviceContext, matrices))
		return false;

	deviceContext->PSSetConstantBuffers(4, 1, &m_viewArrayBuffer);

	return true;
}
bool ConstantBufferStorage::SetPSProjectionMatrixArray(ID3D11DeviceContext * deviceContext, DirectX::XMMATRIX * matrices)
{
	if (!MapProjectionMatrixArray(deviceContext, matrices))
		return false;

	deviceContext->PSSetConstantBuffers(5, 1, &m_projectionArrayBuffer);

	return true;
}
bool ConstantBufferStorage::SetPSPointLight(ID3D11DeviceContext * deviceContext, const Vector3f & position, float intensity)
{
	if (!MapPointLight(deviceContext, position, intensity))
		return false;

	deviceContext->PSSetConstantBuffers(2, 1, &m_pointLightBuffer);

	return true;
}
bool ConstantBufferStorage::SetPSPointLightArray(ID3D11DeviceContext * deviceContext, Vector3f * positions, float * intensities)
{
	if (!MapPointLightArray(deviceContext, positions, intensities))
		return false;

	deviceContext->PSSetConstantBuffers(3, 1, &m_pointLightArrayBuffer);

	return true;
}
bool ConstantBufferStorage::SetPSNrOfLights(ID3D11DeviceContext * deviceContext, int nrOfLights)
{
	if (!MapNrOfLights(deviceContext, nrOfLights))
		return false;
	
	deviceContext->PSSetConstantBuffers(6, 1, &m_nrOfLightsBuffer);

	return true;
}

bool ConstantBufferStorage::MapWorldMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & matrix)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	Matrix* data;

	if (FAILED(deviceContext->Map(m_worldBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	data = static_cast<Matrix*>(mappedResource.pData);
	
	// memcpy here instead of regular assignment bumped frames from 129 to 135

	//data->matrix = matrix;
	memcpy(&data->matrix, &matrix, sizeof(matrix));

	deviceContext->Unmap(m_worldBuffer, 0);
	
	return true;
}
bool ConstantBufferStorage::MapViewMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & matrix)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	Matrix* data;

	if (FAILED(deviceContext->Map(m_viewBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	data = static_cast<Matrix*>(mappedResource.pData);

	// memcpy here instead of regular assignment didn't change frames from ~130
	data->matrix = matrix;

	deviceContext->Unmap(m_viewBuffer, 0);
	
	return true;
}
bool ConstantBufferStorage::MapProjectionMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & matrix)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	Matrix* data;

	if (FAILED(deviceContext->Map(m_projectionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	data = static_cast<Matrix*>(mappedResource.pData);

	// memcpy here instead of regular assignment didn't change frames from ~130
	data->matrix = matrix;

	deviceContext->Unmap(m_projectionBuffer, 0);

	return true;
}
bool ConstantBufferStorage::MapWorldMatrixArray(ID3D11DeviceContext * deviceContext, DirectX::XMMATRIX * matrices)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixChunk* data;

	if (FAILED(deviceContext->Map(m_worldArrayBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	data = static_cast<MatrixChunk*>(mappedResource.pData);

	// This memcpy gave 267 frames in deferred chunk
	// Looping through each matrix and memcpy each gave 257
	// Looping through each matrix and using regular assignment gave 248 frames
	memcpy(data->matrices, matrices, sizeof(matrices[0]) * CHUNK_SIZE);

	deviceContext->Unmap(m_worldArrayBuffer, 0);

	return true;
}
bool ConstantBufferStorage::MapViewMatrixArray(ID3D11DeviceContext * deviceContext, DirectX::XMMATRIX * matrices)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixArray* data;

	if (FAILED(deviceContext->Map(m_viewArrayBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	data = static_cast<MatrixArray*>(mappedResource.pData);

	for (unsigned int i = 0; i < MAX_NR_OF_LIGHTS; i++)
	{
		data->matrices[i] = matrices[i];
	}

	deviceContext->Unmap(m_viewArrayBuffer, 0);

	return true;
}
bool ConstantBufferStorage::MapProjectionMatrixArray(ID3D11DeviceContext * deviceContext, DirectX::XMMATRIX * matrices)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixArray* data;

	if (FAILED(deviceContext->Map(m_projectionArrayBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	data = static_cast<MatrixArray*>(mappedResource.pData);

	for (unsigned int i = 0; i < MAX_NR_OF_LIGHTS; i++)
	{
		data->matrices[i] = matrices[i];
	}

	deviceContext->Unmap(m_projectionArrayBuffer, 0);

	return true;
}
bool ConstantBufferStorage::MapPointLight(ID3D11DeviceContext * deviceContext, const Vector3f & position, float intensity)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	Float4* data;

	if (FAILED(deviceContext->Map(m_pointLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	data = static_cast<Float4*>(mappedResource.pData);
	data->f[0] = position.x;
	data->f[1] = position.y;
	data->f[2] = position.z;
	data->f[3] = intensity;

	deviceContext->Unmap(m_pointLightBuffer, 0);

	return true;
}
bool ConstantBufferStorage::MapPointLightArray(ID3D11DeviceContext * deviceContext, Vector3f * positions, float * intensities)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	Float4Array* data;

	if (FAILED(deviceContext->Map(m_pointLightArrayBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	data = static_cast<Float4Array*>(mappedResource.pData);
	
	for (unsigned int i = 0; i < MAX_NR_OF_LIGHTS; i++)
	{
		data->floats[i].f[0] = positions[i].x;
		data->floats[i].f[1] = positions[i].y;
		data->floats[i].f[2] = positions[i].z;
		data->floats[i].f[3] = intensities[i];
	}

	deviceContext->Unmap(m_pointLightArrayBuffer, 0);

	return true;
}
bool ConstantBufferStorage::MapColor(ID3D11DeviceContext * deviceContext, const Vector3f & color)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	Float4* data;

	if (FAILED(deviceContext->Map(m_colorBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	data = static_cast<Float4*>(mappedResource.pData);
	data->f[0] = color.x;
	data->f[1] = color.y;
	data->f[2] = color.z;
	data->f[3] = 0.0f;

	deviceContext->Unmap(m_colorBuffer, 0);

	return true;
}
bool ConstantBufferStorage::MapNrOfLights(ID3D11DeviceContext * deviceContext, int nrOfLights)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	Int4* data;

	if (FAILED(deviceContext->Map(m_nrOfLightsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	data = static_cast<Int4*>(mappedResource.pData);

	data->i[0] = nrOfLights;

	deviceContext->Unmap(m_nrOfLightsBuffer, 0);

	return true;
}
bool ConstantBufferStorage::MapNrOfObjects(ID3D11DeviceContext * deviceContext, int nrOfObjects)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	Int4* data;

	if (FAILED(deviceContext->Map(m_nrOfObjectsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	data = static_cast<Int4*>(mappedResource.pData);

	data->i[0] = nrOfObjects;

	deviceContext->Unmap(m_nrOfObjectsBuffer, 0);

	return true;
}
