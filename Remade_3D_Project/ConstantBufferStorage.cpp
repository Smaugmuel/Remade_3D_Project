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

	desc.ByteWidth = sizeof(Float4);
	if (FAILED(device->CreateBuffer(&desc, nullptr, &m_colorBuffer)))
		return false;

	return true;
}

bool ConstantBufferStorage::SetWorldMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & matrix)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	Matrix* data;

	if (FAILED(deviceContext->Map(m_worldBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	data = static_cast<Matrix*>(mappedResource.pData);
	data->matrix = matrix;

	deviceContext->Unmap(m_worldBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &m_worldBuffer);

	return true;
}
bool ConstantBufferStorage::SetViewMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & matrix)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	Matrix* data;

	if (FAILED(deviceContext->Map(m_viewBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	data = static_cast<Matrix*>(mappedResource.pData);
	data->matrix = matrix;

	deviceContext->Unmap(m_viewBuffer, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &m_viewBuffer);

	return true;
}
bool ConstantBufferStorage::SetProjectionMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & matrix)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	Matrix* data;

	if (FAILED(deviceContext->Map(m_projectionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	data = static_cast<Matrix*>(mappedResource.pData);
	data->matrix = matrix;

	deviceContext->Unmap(m_projectionBuffer, 0);
	deviceContext->VSSetConstantBuffers(2, 1, &m_projectionBuffer);

	return true;
}

bool ConstantBufferStorage::SetPointLight(ID3D11DeviceContext * deviceContext, const Vector3f & position, float intensity)
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
	deviceContext->VSSetConstantBuffers(3, 1, &m_pointLightBuffer);

	return true;
}

bool ConstantBufferStorage::SetColor(ID3D11DeviceContext * deviceContext, const Vector3f & color)
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
	deviceContext->VSSetConstantBuffers(4, 1, &m_colorBuffer);

	return true;
}