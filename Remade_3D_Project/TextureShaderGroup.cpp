#include "TextureShaderGroup.hpp"
#include <d3d11.h>

#include "SamplerStorage.hpp"
#include "ConstantBufferTypes.hpp"

TextureShaderGroup::TextureShaderGroup()
{
}

TextureShaderGroup::~TextureShaderGroup()
{
}

bool TextureShaderGroup::Initialize(ID3D11Device * device)
{
	D3D11_BUFFER_DESC desc;
	 
	m_vertexShaderName = "VS_Texture.hlsl";
	m_pixelShaderName = "PS_Texture.hlsl";
	
	if (!ShaderGroup::Initialize(device))
		return false;
	
	m_samplerName = "PointClamp";

	memset(&desc, 0, sizeof(desc));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(Float4);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	if (FAILED(device->CreateBuffer(&desc, nullptr, &m_pointLightBuffer)))
	{
		return false;
	}

	return true;
}

void TextureShaderGroup::SetupShaders(ID3D11DeviceContext * deviceContext)
{
	ShaderGroup::SetupShaders(deviceContext);

	ID3D11SamplerState* sampler = SamplerStorage::Get()->GetSampler(m_samplerName);
	deviceContext->PSSetSamplers(0, 1, &sampler);
}

void TextureShaderGroup::SetupPerFrameBuffer(ID3D11DeviceContext * deviceContext, Vector3f lightPosition, float lightIntensity)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result;
	
	Float4* pointLightData;

	result = deviceContext->Map(
		m_pointLightBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource
	);
	if (FAILED(result))
	{
		return;
	}

	pointLightData = (Float4*)mappedResource.pData;
	pointLightData->f[0] = lightPosition.x;
	pointLightData->f[1] = lightPosition.y;
	pointLightData->f[2] = lightPosition.z;
	pointLightData->f[3] = lightIntensity;

	deviceContext->Unmap(m_pointLightBuffer, 0);
	deviceContext->VSSetConstantBuffers(3, 1, &m_pointLightBuffer);
}

void TextureShaderGroup::SetupPerObjectBuffer(ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture)
{
	deviceContext->PSSetShaderResources(0, 1, &texture);
}