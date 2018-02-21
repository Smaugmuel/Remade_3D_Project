#include "DeferredTextureShaderGroup.hpp"
#include <d3d11.h>

#include "SamplerStorage.hpp"

DeferredTextureShaderGroup::DeferredTextureShaderGroup()
{
}

DeferredTextureShaderGroup::~DeferredTextureShaderGroup()
{
}

bool DeferredTextureShaderGroup::Initialize(ID3D11Device * device)
{
	m_vertexShaderName = "VS_D_Texture.hlsl";
	m_pixelShaderName = "PS_D_Texture.hlsl";

	if (!ShaderGroup::Initialize(device))
		return false;

	m_samplerName = "PointClamp";

	return true;
}

void DeferredTextureShaderGroup::SetupShaders(ID3D11DeviceContext * deviceContext)
{
	ShaderGroup::SetupShaders(deviceContext);

	ID3D11SamplerState* sampler = SamplerStorage::Get()->GetSampler(m_samplerName);
	deviceContext->PSSetSamplers(0, 1, &sampler);
}

void DeferredTextureShaderGroup::SetupPerFrameBuffer(ID3D11DeviceContext * deviceContext)
{
}

void DeferredTextureShaderGroup::SetupPerObjectBuffer(ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture)
{
	deviceContext->PSSetShaderResources(0, 1, &texture);
}
