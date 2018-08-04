#include "DeferredLightMultipleShadowLightsShaderGroup.hpp"
#include <d3d11.h>

#include "../../SamplerStorage.hpp"

#include "../../../Lights/PointLightManager.hpp"

DeferredLightMultipleShadowLightsShaderGroup::DeferredLightMultipleShadowLightsShaderGroup()
{
}

DeferredLightMultipleShadowLightsShaderGroup::~DeferredLightMultipleShadowLightsShaderGroup()
{
}

bool DeferredLightMultipleShadowLightsShaderGroup::Initialize(ID3D11Device * device)
{
	m_vertexShaderName = "VS_PosUV.hlsl";
	m_geometryShaderName = "NULL";
	m_pixelShaderName = "PS_D_LightMultipleShadowLights.hlsl";

	if (!ShaderGroup::Initialize(device))
		return false;

	m_samplerName = "PointClamp";

	return true;
}

void DeferredLightMultipleShadowLightsShaderGroup::SetupShaders(ID3D11DeviceContext * deviceContext)
{
	ShaderGroup::SetupShaders(deviceContext);

	ID3D11SamplerState* sampler = SamplerStorage::Get()->GetSampler(m_samplerName);
	deviceContext->PSSetSamplers(0, 1, &sampler);
}

void DeferredLightMultipleShadowLightsShaderGroup::SetupPerFrameBuffer(ID3D11DeviceContext * deviceContext, unsigned int nrOfResources, ID3D11ShaderResourceView ** resources, ID3D11ShaderResourceView ** depthTextures)
{
	deviceContext->PSSetShaderResources(0, nrOfResources, resources);
	deviceContext->PSSetShaderResources(nrOfResources, PointLightManager::Get()->GetNrOfPointLights(), depthTextures);
}
