#include "TextureShaderGroup.hpp"
#include "Camera.hpp"

#include <d3d11.h>

#include "ShaderStorage.hpp"

TextureShaderGroup::TextureShaderGroup()
{
}

TextureShaderGroup::~TextureShaderGroup()
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

bool TextureShaderGroup::Initialize(ID3D11Device * device)
{
	HRESULT result;
	D3D11_BUFFER_DESC vs_perObjectDesc;
	D3D11_BUFFER_DESC vs_perFrameDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	 
	m_vertexShaderName = "VS_Texture.hlsl";
	m_pixelShaderName = "PS_Texture.hlsl";
	
	if (!ShaderStorage::Get()->CreateVertexShader(device, m_vertexShaderName))
		return false;
	if (!ShaderStorage::Get()->CreatePixelShader(device, m_pixelShaderName))
		return false;

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

	// Create per-object vertex shader constant buffer ==========================================================
	memset(&vs_perObjectDesc, 0, sizeof(vs_perObjectDesc));
	vs_perObjectDesc.Usage = D3D11_USAGE_DYNAMIC;
	vs_perObjectDesc.ByteWidth = sizeof(VS_PerObjectBuffer);
	vs_perObjectDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	vs_perObjectDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vs_perObjectDesc.MiscFlags = 0;
	vs_perObjectDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&vs_perObjectDesc, nullptr, &m_vsPerObjectBuffer);//&m_vsBuffers[1]);
	if (FAILED(result))
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

	result = device->CreateBuffer(&vs_perFrameDesc, nullptr, &m_vsPerFrameBuffer);//&m_vsBuffers[0]);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void TextureShaderGroup::SetupShaders(ID3D11DeviceContext * deviceContext)
{
	ShaderStorage* storage = ShaderStorage::Get();

	deviceContext->VSSetShader(storage->GetVertexShader(m_vertexShaderName), nullptr, 0);
	deviceContext->HSSetShader(nullptr, nullptr, 0);
	deviceContext->DSSetShader(nullptr, nullptr, 0);
	deviceContext->GSSetShader(nullptr, nullptr, 0);
	deviceContext->PSSetShader(storage->GetPixelShader(m_pixelShaderName), nullptr, 0);

	deviceContext->IASetInputLayout(storage->GetInputLayout(m_vertexShaderName));

	deviceContext->PSSetSamplers(0, 1, &m_samplerState);
}

void TextureShaderGroup::SetupPerFrameBuffer(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & viewMatrix, const DirectX::XMMATRIX & projectionMatrix, Vector3f lightPosition, float lightIntensity)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result;
	VS_PerFrameBuffer* frameDataVS;

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
	frameDataVS->lightPosition = lightPosition;
	frameDataVS->padding = lightIntensity;

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

void TextureShaderGroup::SetupPerObjectBuffer(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & worldMatrix, ID3D11ShaderResourceView * texture)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result;
	VS_PerObjectBuffer* objectData;

	// Mapping and updating PerFrameConstantBuffer
	result = deviceContext->Map(
		m_vsPerObjectBuffer,//m_vsBuffers[0],
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

	deviceContext->Unmap(m_vsPerObjectBuffer, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &m_vsPerObjectBuffer);

	deviceContext->PSSetShaderResources(0, 1, &texture);
}