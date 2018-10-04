#include "DeferredTextureChunkShaderGroup.hpp"
#include <d3d11.h>

//#include "../../SamplerStorage.hpp"

DeferredTextureChunkShaderGroup::DeferredTextureChunkShaderGroup()
{
}

DeferredTextureChunkShaderGroup::~DeferredTextureChunkShaderGroup()
{
}

bool DeferredTextureChunkShaderGroup::Initialize(ID3D11Device * device)
{
	m_vertexShaderName = "VS_D_TextureChunk.hlsl";
	m_geometryShaderName = "GS_D_TextureChunk.hlsl";
	m_pixelShaderName = "PS_D_Texture.hlsl";

	if (!ShaderGroup::Initialize(device))
		return false;

	m_samplerName = "PointClamp";

	return true;
}

void DeferredTextureChunkShaderGroup::SetupShaders(ID3D11DeviceContext * deviceContext)
{
	ShaderGroup::SetupShaders(deviceContext);

	/*ID3D11SamplerState* sampler = SamplerStorage::Get()->GetSampler(m_samplerName);
	deviceContext->PSSetSamplers(0, 1, &sampler);*/
}

void DeferredTextureChunkShaderGroup::SetupPerFrameBuffer(ID3D11DeviceContext * deviceContext)
{
}

void DeferredTextureChunkShaderGroup::SetupPerObjectBuffer(ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture)
{
	deviceContext->PSSetShaderResources(0, 1, &texture);
}