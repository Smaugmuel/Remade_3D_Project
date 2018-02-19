#include "DeferredSingleColorShaderGroup.hpp"
#include <d3d11.h>

DeferredSingleColorShaderGroup::DeferredSingleColorShaderGroup()
{
}

DeferredSingleColorShaderGroup::~DeferredSingleColorShaderGroup()
{
	if (m_vsPerFrameBuffer)
	{
		m_vsPerFrameBuffer->Release();
		m_vsPerFrameBuffer = nullptr;
	}
	if (m_vsPerObjectBuffer)
	{
		m_vsPerObjectBuffer->Release();
		m_vsPerObjectBuffer = nullptr;
	}
}

bool DeferredSingleColorShaderGroup::Initialize(ID3D11Device * device)
{
	D3D11_BUFFER_DESC vs_perObjectDesc;
	D3D11_BUFFER_DESC vs_perFrameDesc;

	m_vertexShaderName = "VS_PosNormColor.hlsl";
	m_pixelShaderName = "PS_D_SingleColor.hlsl";

	if (!ShaderGroup::Initialize(device))
		return false;


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

	return true;
}

void DeferredSingleColorShaderGroup::SetupShaders(ID3D11DeviceContext * deviceContext)
{
	ShaderGroup::SetupShaders(deviceContext);

	// Might need to set sample state here
}

void DeferredSingleColorShaderGroup::SetupPerFrameBuffer(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & viewMatrix, const DirectX::XMMATRIX & projectionMatrix)
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
	frameDataVS->view = viewMatrix;
	frameDataVS->projection = projectionMatrix;

	deviceContext->Unmap(m_vsPerFrameBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &m_vsPerFrameBuffer);
}

void DeferredSingleColorShaderGroup::SetupPerObjectBuffer(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & worldMatrix, Vector3f color)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result;
	VS_PerObjectBuffer* objectData;

	// Mapping and updating PerFrameConstantBuffer
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
	objectData->color = color;
	objectData->padding = 0.0f;

	deviceContext->Unmap(m_vsPerObjectBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &m_vsPerObjectBuffer);
}