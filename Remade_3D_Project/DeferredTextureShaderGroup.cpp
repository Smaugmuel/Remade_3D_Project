#include "DeferredTextureShaderGroup.hpp"
#include <d3d11.h>

#include "SamplerStorage.hpp"

DeferredTextureShaderGroup::DeferredTextureShaderGroup()
{
}

DeferredTextureShaderGroup::~DeferredTextureShaderGroup()
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

bool DeferredTextureShaderGroup::Initialize(ID3D11Device * device)
{
	D3D11_BUFFER_DESC vs_perObjectDesc;
	D3D11_BUFFER_DESC vs_perFrameDesc;

	m_vertexShaderName = "VS_D_Texture.hlsl";
	m_pixelShaderName = "PS_D_Texture.hlsl";

	if (!ShaderGroup::Initialize(device))
		return false;

	m_samplerName = "PointClamp";

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

	// Create per-frame pixel shader constant buffer ==========================================================
	//memset(&ps_perFrameDesc, 0, sizeof(ps_perFrameDesc));
	//ps_perFrameDesc.Usage = D3D11_USAGE_DYNAMIC;
	//ps_perFrameDesc.ByteWidth = sizeof(PS_PerFrameBuffer);
	//ps_perFrameDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//ps_perFrameDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//ps_perFrameDesc.MiscFlags = 0;
	//ps_perFrameDesc.StructureByteStride = 0;

	//result = device->CreateBuffer(&ps_perFrameDesc, nullptr, &m_psPerFrameBuffer);//&m_vsBuffers[1]);
	//if (FAILED(result))
	//{
	//	return false;
	//}

	return true;
}

void DeferredTextureShaderGroup::SetupShaders(ID3D11DeviceContext * deviceContext)
{
	ShaderGroup::SetupShaders(deviceContext);

	ID3D11SamplerState* sampler = SamplerStorage::Get()->GetSampler(m_samplerName);
	deviceContext->PSSetSamplers(0, 1, &sampler);
}

void DeferredTextureShaderGroup::SetupPerFrameBuffer(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & viewMatrix, const DirectX::XMMATRIX & projectionMatrix)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result;
	VS_PerFrameBuffer* frameDataVS;
	//PS_PerFrameBuffer* frameDataPS;

	// Vertex Shader ===========================================================================
	// Mapping and updating buffer
	result = deviceContext->Map(
		m_vsPerFrameBuffer,//m_vsBuffers[0],
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


	// Pixel Shader ===========================================================================
	// Mapping and updating buffer
	//result = deviceContext->Map(
	//	m_psPerFrameBuffer,//m_vsBuffers[0],
	//	0,
	//	D3D11_MAP_WRITE_DISCARD,
	//	0,
	//	&mappedResource
	//	);
	//if (FAILED(result))
	//{
	//	return;
	//}

	//frameDataPS = (PS_PerFrameBuffer*)mappedResource.pData;
	//frameDataPS->lightPosition = lightCamera->GetPosition();
	//frameDataPS->lightIntensity = lightIntensity;

	//deviceContext->Unmap(m_psPerFrameBuffer, 0);
	//deviceContext->PSSetConstantBuffers(0, 1, &m_psPerFrameBuffer);
}

void DeferredTextureShaderGroup::SetupPerObjectBuffer(ID3D11DeviceContext * deviceContext, /*const DirectX::XMMATRIX & worldMatrix,*/ ID3D11ShaderResourceView * texture)
{
	//D3D11_MAPPED_SUBRESOURCE mappedResource;
	//HRESULT result;
	//VS_PerObjectBuffer* objectData;

	//// Mapping and updating PerObjectConstantBuffer
	//result = deviceContext->Map(
	//	m_vsPerObjectBuffer,//m_vsBuffers[0],
	//	0,
	//	D3D11_MAP_WRITE_DISCARD,
	//	0,
	//	&mappedResource
	//	);
	//if (FAILED(result))
	//{
	//	return;
	//}


	//objectData = (VS_PerObjectBuffer*)mappedResource.pData;
	//objectData->world = worldMatrix;

	//deviceContext->Unmap(m_vsPerObjectBuffer, 0);
	//deviceContext->VSSetConstantBuffers(1, 1, &m_vsPerObjectBuffer);

	deviceContext->PSSetShaderResources(0, 1, &texture);
}
