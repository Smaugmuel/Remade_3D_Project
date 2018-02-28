#include "HUDShaderGroup.hpp"
#include <d3d11.h>

#include "SamplerStorage.hpp"

HUDShaderGroup::HUDShaderGroup()
{
}

HUDShaderGroup::~HUDShaderGroup()
{
}

bool HUDShaderGroup::Initialize(ID3D11Device * device)
{
	m_vertexShaderName = "VS_PosUV.hlsl";
	m_geometryShaderName = "NULL";
	m_pixelShaderName = "PS_HUD.hlsl";

	if (!ShaderGroup::Initialize(device))
		return false;

	m_samplerName = "PointClamp";

	return true;
}

void HUDShaderGroup::SetupShaders(ID3D11DeviceContext * deviceContext)
{
	ShaderGroup::SetupShaders(deviceContext);

	ID3D11SamplerState* sampler = SamplerStorage::Get()->GetSampler(m_samplerName);
	deviceContext->PSSetSamplers(0, 1, &sampler);
}

void HUDShaderGroup::SetupPerObjectBuffer(ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture)
{
	deviceContext->PSSetShaderResources(0, 1, &texture);
}