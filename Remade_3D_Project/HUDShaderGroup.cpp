#include "HUDShaderGroup.hpp"
#include <d3d11.h>

#include "ShaderStorage.hpp"
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
	m_pixelShaderName = "PS_HUD.hlsl";
	m_samplerName = "PointClamp";

	if (!ShaderStorage::Get()->CreateVertexShader(device, m_vertexShaderName))
		return false;
	if (!ShaderStorage::Get()->CreatePixelShader(device, m_pixelShaderName))
		return false;

	return true;
}

void HUDShaderGroup::SetupShaders(ID3D11DeviceContext * deviceContext)
{
	ShaderStorage* storage = ShaderStorage::Get();

	deviceContext->VSSetShader(storage->GetVertexShader(m_vertexShaderName), nullptr, 0);
	deviceContext->HSSetShader(nullptr, nullptr, 0);
	deviceContext->DSSetShader(nullptr, nullptr, 0);
	deviceContext->GSSetShader(nullptr, nullptr, 0);
	deviceContext->PSSetShader(storage->GetPixelShader(m_pixelShaderName), nullptr, 0);

	deviceContext->IASetInputLayout(storage->GetInputLayout(m_vertexShaderName));

	ID3D11SamplerState* sampler = SamplerStorage::Get()->GetSampler(m_samplerName);
	deviceContext->PSSetSamplers(0, 1, &sampler);
}

void HUDShaderGroup::SetupPerObjectBuffer(ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture)
{
	deviceContext->PSSetShaderResources(0, 1, &texture);
}