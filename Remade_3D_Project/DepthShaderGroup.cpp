#include "DepthShaderGroup.hpp"
#include <d3d11.h>
#include <d3dcompiler.h>
#include "Camera.hpp"
#include "Object.hpp"

DepthShaderGroup::DepthShaderGroup()
{
}

DepthShaderGroup::~DepthShaderGroup()
{
	if (m_layout)
	{
		m_layout->Release();
		m_layout = nullptr;
	}
	if (m_vsPerObjectBuffer)
	{
		m_vsPerObjectBuffer->Release();
		m_vsPerObjectBuffer = nullptr;
	}
	if (m_vsPerFrameBuffer)
	{
		m_vsPerFrameBuffer->Release();
		m_vsPerFrameBuffer = nullptr;
	}
	if (m_ps)
	{
		m_ps->Release();
		m_ps = nullptr;
	}
	if (m_vs)
	{
		m_vs->Release();
		m_vs = nullptr;
	}
}

bool DepthShaderGroup::Initialize(ID3D11Device * device)
{
	ID3D10Blob* vertexShaderBlob;
	ID3D10Blob* pixelShaderBlob;
	HRESULT result;
	D3D11_BUFFER_DESC vs_perObjectDesc;
	D3D11_BUFFER_DESC vs_perFrameDesc;


	wchar_t* vsName = L"VS_Depth.hlsl";
	wchar_t* psName = L"PS_Depth.hlsl";



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

void DepthShaderGroup::SetupShaders(ID3D11DeviceContext * deviceContext)
{
	deviceContext->VSSetShader(m_vs, nullptr, 0);
	deviceContext->HSSetShader(nullptr, nullptr, 0);
	deviceContext->DSSetShader(nullptr, nullptr, 0);
	deviceContext->GSSetShader(nullptr, nullptr, 0);
	deviceContext->PSSetShader(m_ps, nullptr, 0);

	deviceContext->IASetInputLayout(m_layout);
}

void DepthShaderGroup::SetupPerFrameBuffer(ID3D11DeviceContext * deviceContext, Camera * camera)
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
	frameDataVS->view = camera->GetViewMatrix();
	frameDataVS->proj = camera->GetProjectionMatrix();

	deviceContext->Unmap(m_vsPerFrameBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &m_vsPerFrameBuffer);
}

void DepthShaderGroup::SetupPerObjectBuffer(ID3D11DeviceContext * deviceContext, Object * object)
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
	objectData->world = object->GetWorldMatrix();

	deviceContext->Unmap(m_vsPerObjectBuffer, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &m_vsPerObjectBuffer);
}