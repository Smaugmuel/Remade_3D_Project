#include "DeferredLightMultipleLightsShaderGroup.hpp"
#include <d3d11.h>

#include "../../SamplerStorage.hpp"

DeferredLightMultipleLightsShaderGroup::DeferredLightMultipleLightsShaderGroup()
{
}

DeferredLightMultipleLightsShaderGroup::~DeferredLightMultipleLightsShaderGroup()
{
}

bool DeferredLightMultipleLightsShaderGroup::Initialize(ID3D11Device* device)
{
	m_vertexShaderName = "VS_PosUV.hlsl";
	m_geometryShaderName = "NULL";
	m_pixelShaderName = "PS_D_LightMultipleLights.hlsl";

	if (!ShaderGroup::Initialize(device))
		return false;

	m_samplerName = "PointClamp";

	return true;
}

void DeferredLightMultipleLightsShaderGroup::SetupShaders(ID3D11DeviceContext * deviceContext)
{
	ShaderGroup::SetupShaders(deviceContext);

	ID3D11SamplerState* sampler = SamplerStorage::Get()->GetSampler(m_samplerName);
	deviceContext->PSSetSamplers(0, 1, &sampler);
}

void DeferredLightMultipleLightsShaderGroup::SetupPerFrameBuffer(ID3D11DeviceContext * deviceContext, unsigned int nrOfResources, ID3D11ShaderResourceView ** resources/*, Vector3f lightPositions[MAX_NR_OF_LIGHTS], float lightIntensitys[MAX_NR_OF_LIGHTS]*/)
{
	deviceContext->PSSetShaderResources(0, nrOfResources, resources);
}