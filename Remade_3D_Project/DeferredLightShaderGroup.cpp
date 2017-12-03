#include "DeferredLightShaderGroup.hpp"
#include <d3d11.h>
#include <d3dcompiler.h>
#include "Camera.hpp"

DeferredLightShaderGroup::DeferredLightShaderGroup()
{
}

DeferredLightShaderGroup::~DeferredLightShaderGroup()
{
}

bool DeferredLightShaderGroup::Initialize(ID3D11Device* device)
{
	ID3D10Blob* vertexShaderBlob;
	ID3D10Blob* pixelShaderBlob;
	HRESULT result;
	D3D11_BUFFER_DESC ps_perFrameDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_INPUT_ELEMENT_DESC inputDesc[2];

	wchar_t* vsName = L"VS_D_Light.hlsl";
	wchar_t* psName = L"PS_D_Light.hlsl";


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
	inputDesc[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	inputDesc[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };

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
	samplerDesc.Filter = /*D3D11_FILTER_MIN_MAG_MIP_LINEAR*/ D3D11_FILTER_MIN_MAG_MIP_POINT;
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

	if (FAILED(device->CreateSamplerState(&samplerDesc, &m_samplerState)))
	{
		return false;
	}


	// Create per-frame constant buffer ===========================================================
	memset(&ps_perFrameDesc, 0, sizeof(ps_perFrameDesc));
	ps_perFrameDesc.Usage = D3D11_USAGE_DYNAMIC;
	ps_perFrameDesc.ByteWidth = sizeof(PS_PerFrameBuffer);
	ps_perFrameDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ps_perFrameDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ps_perFrameDesc.MiscFlags = 0;
	ps_perFrameDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&ps_perFrameDesc, nullptr, &m_psPerFrameBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void DeferredLightShaderGroup::SetupShaders(ID3D11DeviceContext * deviceContext)
{
	deviceContext->VSSetShader(m_vs, nullptr, 0);
	deviceContext->HSSetShader(nullptr, nullptr, 0);
	deviceContext->DSSetShader(nullptr, nullptr, 0);
	deviceContext->GSSetShader(nullptr, nullptr, 0);
	deviceContext->PSSetShader(m_ps, nullptr, 0);

	deviceContext->IASetInputLayout(m_layout);
	deviceContext->PSSetSamplers(0, 1, &m_samplerState);
}

void DeferredLightShaderGroup::SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext, unsigned int nrOfResources, ID3D11ShaderResourceView** resources, /*ID3D11ShaderResourceView* depthTexture,*/ Camera* lightCamera, float lightIntensity)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	PS_PerFrameBuffer* frameDataPS;
	HRESULT result;

	// Pixel Shader ===========================================================================
	// Mapping and updating buffer
	result = deviceContext->Map(
		m_psPerFrameBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource
		);
	if (FAILED(result))
	{
		return;
	}
	
	frameDataPS = (PS_PerFrameBuffer*)mappedResource.pData;
	frameDataPS->lightPosition = lightCamera->GetPosition();
	frameDataPS->lightIntensity = lightIntensity;
	frameDataPS->lightView = lightCamera->GetViewMatrix();
	frameDataPS->lightProj = lightCamera->GetProjectionMatrix();

	deviceContext->Unmap(m_psPerFrameBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &m_psPerFrameBuffer);

	deviceContext->PSSetShaderResources(0, nrOfResources, resources);
	//deviceContext->PSGetShaderResources(nrOfResources, 1, &depthTexture);
}