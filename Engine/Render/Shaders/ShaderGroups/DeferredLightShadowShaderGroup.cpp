#include "DeferredLightShadowShaderGroup.hpp"
#include <d3d11.h>

//#include "../../SamplerStorage.hpp"

DeferredLightShadowShaderGroup::DeferredLightShadowShaderGroup()
{
}

DeferredLightShadowShaderGroup::~DeferredLightShadowShaderGroup()
{
}

bool DeferredLightShadowShaderGroup::Initialize(ID3D11Device* device)
{
	m_vertexShaderName = "VS_PosUV.hlsl";
	m_geometryShaderName = "NULL";
	m_pixelShaderName = "PS_D_Light.hlsl";
	
	if (!ShaderGroup::Initialize(device))
		return false;

	m_samplerName = "PointClamp";

	return true;
}

void DeferredLightShadowShaderGroup::SetupShaders(ID3D11DeviceContext * deviceContext)
{
	ShaderGroup::SetupShaders(deviceContext);

	/*ID3D11SamplerState* sampler = SamplerStorage::Get()->GetSampler(m_samplerName);
	deviceContext->PSSetSamplers(0, 1, &sampler);*/
}

void DeferredLightShadowShaderGroup::SetupPerFrameBuffer(ID3D11DeviceContext * deviceContext, unsigned int nrOfResources, ID3D11ShaderResourceView ** resources, ID3D11ShaderResourceView * depthTexture/*, Vector3f lightPosition, const DirectX::XMMATRIX & lightViewMatrix, const DirectX::XMMATRIX & lightProjectionMatrix, float lightIntensity*/)
{
	deviceContext->PSSetShaderResources(0, nrOfResources, resources);
	deviceContext->PSSetShaderResources(nrOfResources, 1, &depthTexture);
}