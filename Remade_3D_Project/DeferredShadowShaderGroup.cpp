#include "DeferredShadowShaderGroup.hpp"
#include "Object.hpp"
#include "Camera.hpp"

#include <d3d11.h>
#include <d3dcompiler.h>

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

	/*ID3D10Blob* vertexShaderBlob;
	ID3D10Blob* pixelShaderBlob;
	HRESULT result;
	D3D11_SAMPLER_DESC samplerDesc;
	wchar_t* vsName = L"VS_Shadow.hlsl";
	wchar_t* psName = L"PS_Shadow.hlsl";


	// Compile shaders ============================================================================
	result = D3DCompileFromFile(
		vsName,
		nullptr,
		nullptr,
		"main",
		"vs_5_0",
		0,
		0,
		&vertexShaderBlob,
		nullptr
		);
	if (FAILED(result))
	{
		return false;
	}

	result = D3DCompileFromFile(
		psName,
		nullptr,
		nullptr,
		"main",
		"ps_5_0",
		0,
		0,
		&pixelShaderBlob,
		nullptr
		);
	if (FAILED(result))
	{
		return false;
	}


	// Create shaders =============================================================================
	result = device->CreateVertexShader(
		vertexShaderBlob->GetBufferPointer(),
		vertexShaderBlob->GetBufferSize(),
		nullptr,
		&m_vs
		);
	if (FAILED(result))
	{
		return false;
	}

	result = device->CreatePixelShader(
		pixelShaderBlob->GetBufferPointer(),
		pixelShaderBlob->GetBufferSize(),
		nullptr,
		&m_ps
		);
	if (FAILED(result))
	{
		return false;
	}


	// Create input layout ========================================================================
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	result = device->CreateInputLayout(
		inputDesc,
		ARRAYSIZE(inputDesc),
		vertexShaderBlob->GetBufferPointer(),
		vertexShaderBlob->GetBufferSize(),
		&m_layout
		);
	if (FAILED(result))
	{
		return false;
	}

	vertexShaderBlob->Release();
	vertexShaderBlob = nullptr;

	pixelShaderBlob->Release();
	pixelShaderBlob = nullptr;


	// Create sampler state =======================================================================
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	if (FAILED(device->CreateSamplerState(&samplerDesc, &m_sampleStateClamp)))
	{
		return false;
	}*/

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
	//deviceContext->VSSetShader(m_vs, nullptr, 0);
	//deviceContext->PSSetShader(m_ps, nullptr, 0);

	//deviceContext->IASetInputLayout(m_layout);
	//deviceContext->PSSetSamplers(0, 1, &m_sampleStateClamp);

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