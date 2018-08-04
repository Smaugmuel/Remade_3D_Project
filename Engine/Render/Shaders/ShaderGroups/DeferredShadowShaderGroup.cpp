#include "DeferredShadowShaderGroup.hpp"
#include <d3d11.h>

DeferredShadowShaderGroup::DeferredShadowShaderGroup()
{
}

DeferredShadowShaderGroup::~DeferredShadowShaderGroup()
{
}

bool DeferredShadowShaderGroup::Initialize(ID3D11Device * device)
{
	D3D11_BUFFER_DESC vs_perFrameDesc;
	D3D11_BUFFER_DESC vs_perObjectDesc;

	// Don't call ShaderGroup::Initialize(), since this class has no vertex or pixel shaders

	// Create per-frame vertex shader constant buffer ===========================================================
	memset(&vs_perFrameDesc, 0, sizeof(vs_perFrameDesc));
	vs_perFrameDesc.Usage = D3D11_USAGE_DYNAMIC;
	vs_perFrameDesc.ByteWidth = sizeof(VS_PerFrameBuffer);
	vs_perFrameDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	vs_perFrameDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vs_perFrameDesc.MiscFlags = 0;
	vs_perFrameDesc.StructureByteStride = 0;

	if (FAILED(device->CreateBuffer(&vs_perFrameDesc, nullptr, &m_vsPerFrameBuffer)))
	{
		return false;
	}

	// Create per-object vertex shader constant buffer ==========================================================
	memset(&vs_perObjectDesc, 0, sizeof(vs_perObjectDesc));
	vs_perObjectDesc.Usage = D3D11_USAGE_DYNAMIC;
	vs_perObjectDesc.ByteWidth = sizeof(VS_PerObjectBuffer);
	vs_perObjectDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	vs_perObjectDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vs_perObjectDesc.MiscFlags = 0;
	vs_perObjectDesc.StructureByteStride = 0;

	if (FAILED(device->CreateBuffer(&vs_perObjectDesc, nullptr, &m_vsPerObjectBuffer)))
	{
		return false;
	}

	return true;
}

void DeferredShadowShaderGroup::SetupShaders(ID3D11DeviceContext * deviceContext)
{
	// Don't call ShaderGroup::SetupShaders(), since this class has no vertex or pixel shaders

	deviceContext->PSSetShader(nullptr, nullptr, 0);
}

void DeferredShadowShaderGroup::SetupPerFrameBuffer(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & lightViewMatrix, const DirectX::XMMATRIX & lightProjectionMatrix)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result;
	VS_PerFrameBuffer* frameDataVS;

	// Vertex Shader ===========================================================================
	// Mapping and updating buffer
	result = deviceContext->Map(
		m_vsPerFrameBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource
		);
	if (FAILED(result))
	{
		return;
	}

	frameDataVS = (VS_PerFrameBuffer*)mappedResource.pData;
	//frameDataVS->view = camera->GetViewMatrix();
	//frameDataVS->projection = camera->GetProjectionMatrix();
	frameDataVS->lightView = lightViewMatrix;
	frameDataVS->lightProj = lightProjectionMatrix;
	//frameDataVS->lightPosition = lightCamera->GetPosition();
	//frameDataVS->padding = 0.0f;

	deviceContext->Unmap(m_vsPerFrameBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &m_vsPerFrameBuffer);
	//deviceContext->PSSetShaderResources(0, 1, &depthTexture);
}

void DeferredShadowShaderGroup::SetupPerObjectBuffer(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX& worldMatrix)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result;
	VS_PerObjectBuffer* objectData;

	// Mapping and updating PerObject constant buffer
	result = deviceContext->Map(
		m_vsPerObjectBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource
		);
	if (FAILED(result))
	{
		return;
	}

	objectData = (VS_PerObjectBuffer*)mappedResource.pData;
	objectData->world = worldMatrix;
	objectData->color = Vector3f(0, 0, 0);
	objectData->padding = 1.0f;

	deviceContext->Unmap(m_vsPerObjectBuffer, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &m_vsPerObjectBuffer);
}