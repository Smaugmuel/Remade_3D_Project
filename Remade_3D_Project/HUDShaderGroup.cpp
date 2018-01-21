#include "HUDShaderGroup.hpp"
#include <d3d11.h>
#include <d3dcompiler.h>

HUDShaderGroup::HUDShaderGroup()
{
}

HUDShaderGroup::~HUDShaderGroup()
{
	if (m_vs)
	{
		m_vs->Release();
		m_vs = nullptr;
	}
	if (m_ps)
	{
		m_ps->Release();
		m_ps = nullptr;
	}
	if (m_layout)
	{
		m_layout->Release();
		m_layout = nullptr;
	}
}

bool HUDShaderGroup::Initialize(ID3D11Device * device)
{
	ID3D10Blob* vertexShaderBlob;
	ID3D10Blob* pixelShaderBlob;
	HRESULT result;
	D3D11_SAMPLER_DESC samplerDesc;


	wchar_t* vsName = L"VS_HUD.hlsl";
	wchar_t* psName = L"PS_HUD.hlsl";


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
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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

	if (FAILED(device->CreateSamplerState(&samplerDesc, &m_samplerState)))
	{
		return false;
	}

	return true;
}

void HUDShaderGroup::SetupShaders(ID3D11DeviceContext * deviceContext)
{
	deviceContext->VSSetShader(m_vs, nullptr, 0);
	deviceContext->HSSetShader(nullptr, nullptr, 0);
	deviceContext->DSSetShader(nullptr, nullptr, 0);
	deviceContext->GSSetShader(nullptr, nullptr, 0);
	deviceContext->PSSetShader(m_ps, nullptr, 0);

	deviceContext->IASetInputLayout(m_layout);
	deviceContext->PSSetSamplers(0, 1, &m_samplerState);
}

void HUDShaderGroup::SetupPerObjectBuffer(ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture)
{
	deviceContext->PSSetShaderResources(0, 1, &texture);
}