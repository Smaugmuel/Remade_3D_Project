#include "ShadowShaderGroup.hpp"
#include <d3d11.h>
#include <d3dcompiler.h>
#include "Object.hpp"
#include "Camera.hpp"

ShadowShaderGroup::ShadowShaderGroup()
{
}

ShadowShaderGroup::~ShadowShaderGroup()
{
	if (m_psPerFrameBuffer)
	{
		m_psPerFrameBuffer->Release();
		m_psPerFrameBuffer = 0;
	}
	if (m_vsPerFrameBuffer2)
	{
		m_vsPerFrameBuffer2->Release();
		m_vsPerFrameBuffer2 = 0;
	}
	if (m_vsPerFrameBuffer1)
	{
		m_vsPerFrameBuffer1->Release();
		m_vsPerFrameBuffer1 = 0;
	}
	if (m_samplerStateClamp)
	{
		m_samplerStateClamp->Release();
		m_samplerStateClamp = 0;
	}
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}
	if (m_ps)
	{
		m_ps->Release();
		m_ps = 0;
	}
	if (m_vs)
	{
		m_vs->Release();
		m_vs = 0;
	}
}

bool ShadowShaderGroup::Initialize(ID3D11Device * device)
{
	ID3D10Blob* vertexShaderBlob;
	ID3D10Blob* pixelShaderBlob;
	HRESULT result;
	D3D11_BUFFER_DESC vs_perFrameDesc1;
	D3D11_BUFFER_DESC vs_perFrameDesc2;
	D3D11_BUFFER_DESC ps_perFrameDesc;
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

	if (FAILED(device->CreateSamplerState(&samplerDesc, &m_samplerStateClamp)))
	{
		return false;
	}


	// Create per-frame 1 vertex shader constant buffer ==========================================================
	memset(&vs_perFrameDesc1, 0, sizeof(vs_perFrameDesc1));
	vs_perFrameDesc1.Usage = D3D11_USAGE_DYNAMIC;
	vs_perFrameDesc1.ByteWidth = sizeof(VS_PerFrame1);
	vs_perFrameDesc1.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	vs_perFrameDesc1.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vs_perFrameDesc1.MiscFlags = 0;
	vs_perFrameDesc1.StructureByteStride = 0;

	if (FAILED(device->CreateBuffer(&vs_perFrameDesc1, nullptr, &m_vsPerFrameBuffer1)))
	{
		return false;
	}

	// Create per-frame 2 vertex shader constant buffer ===========================================================
	memset(&vs_perFrameDesc2, 0, sizeof(vs_perFrameDesc2));
	vs_perFrameDesc2.Usage = D3D11_USAGE_DYNAMIC;
	vs_perFrameDesc2.ByteWidth = sizeof(VS_PerFrame2);
	vs_perFrameDesc2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	vs_perFrameDesc2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vs_perFrameDesc2.MiscFlags = 0;
	vs_perFrameDesc2.StructureByteStride = 0;

	if (FAILED(device->CreateBuffer(&vs_perFrameDesc2, nullptr, &m_vsPerFrameBuffer2)))
	{
		return false;
	}


	// Create per-frame vertex shader constant buffer ===========================================================
	memset(&ps_perFrameDesc, 0, sizeof(ps_perFrameDesc));
	ps_perFrameDesc.Usage = D3D11_USAGE_DYNAMIC;
	ps_perFrameDesc.ByteWidth = sizeof(PS_PerFrame);
	ps_perFrameDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ps_perFrameDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ps_perFrameDesc.MiscFlags = 0;
	ps_perFrameDesc.StructureByteStride = 0;

	if (FAILED(device->CreateBuffer(&ps_perFrameDesc, nullptr, &m_psPerFrameBuffer)))
	{
		return false;
	}

	return true;
}

void ShadowShaderGroup::SetupShader(ID3D11DeviceContext * deviceContext)
{
	deviceContext->VSSetShader(m_vs, nullptr, 0);
	deviceContext->HSSetShader(nullptr, nullptr, 0);
	deviceContext->DSSetShader(nullptr, nullptr, 0);
	deviceContext->GSSetShader(nullptr, nullptr, 0);
	deviceContext->PSSetShader(m_ps, nullptr, 0);

	deviceContext->IASetInputLayout(m_layout);
	deviceContext->PSSetSamplers(0, 1, &m_samplerStateClamp);
}

void ShadowShaderGroup::SetupPerFrameBuffer(ID3D11DeviceContext * deviceContext, Object* object, Camera* camera, Camera* lightCamera, ID3D11ShaderResourceView* depthTexture)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VS_PerFrame1* frameDataVS1;
	VS_PerFrame2* frameDataVS2;
	PS_PerFrame* frameDataPS;
	HRESULT result;

	// Vertex Shader 1 ===========================================================================
	// Mapping and updating buffer
	result = deviceContext->Map(
		m_vsPerFrameBuffer1,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource
		);
	if (FAILED(result))
	{
		return;
	}

	frameDataVS1 = (VS_PerFrame1*)mappedResource.pData;
	frameDataVS1->world = object->GetWorldMatrix();
	frameDataVS1->view = camera->GetViewMatrix();
	frameDataVS1->proj = camera->GetProjectionMatrix();
	frameDataVS1->lightView = lightCamera->GetViewMatrix();
	frameDataVS1->lightProj = lightCamera->GetProjectionMatrix();

	deviceContext->Unmap(m_vsPerFrameBuffer1, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &m_vsPerFrameBuffer1);

	// Vertex Shader 2 ===========================================================================
	// Mapping and updating buffer
	result = deviceContext->Map(
		m_vsPerFrameBuffer2,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource
		);
	if (FAILED(result))
	{
		return;
	}

	frameDataVS2 = (VS_PerFrame2*)mappedResource.pData;
	frameDataVS2->lightPosition = lightCamera->GetPosition().XMF();
	frameDataVS2->padding = 0.0f;

	deviceContext->Unmap(m_vsPerFrameBuffer2, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &m_vsPerFrameBuffer2);

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

	frameDataPS = (PS_PerFrame*)mappedResource.pData;
	frameDataPS->ambientColor = DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	frameDataPS->diffuseColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	deviceContext->Unmap(m_psPerFrameBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &m_psPerFrameBuffer);
	deviceContext->PSSetShaderResources(0, 1, &depthTexture);
}