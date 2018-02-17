#include "DeferredLightShadowShaderGroup.hpp"
#include <d3d11.h>

#include "SamplerStorage.hpp"

DeferredLightShadowShaderGroup::DeferredLightShadowShaderGroup()
{
}

DeferredLightShadowShaderGroup::~DeferredLightShadowShaderGroup()
{
}

bool DeferredLightShadowShaderGroup::Initialize(ID3D11Device* device)
{
	D3D11_BUFFER_DESC ps_perFrameDesc;

	m_vertexShaderName = "VS_PosUV.hlsl";
	m_pixelShaderName = "PS_D_Light.hlsl";
	
	if (!ShaderGroup::Initialize(device))
		return false;

	m_samplerName = "PointClamp";


	// Create per-frame constant buffer ===========================================================
	memset(&ps_perFrameDesc, 0, sizeof(ps_perFrameDesc));
	ps_perFrameDesc.Usage = D3D11_USAGE_DYNAMIC;
	ps_perFrameDesc.ByteWidth = sizeof(PS_PerFrameBuffer);
	ps_perFrameDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ps_perFrameDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ps_perFrameDesc.MiscFlags = 0;
	ps_perFrameDesc.StructureByteStride = 0;

	if (FAILED(device->CreateBuffer(&ps_perFrameDesc, nullptr, &m_psPerFrameBuffer)))
	{
		return false;
	}

	return true;
}

void DeferredLightShadowShaderGroup::SetupShaders(ID3D11DeviceContext * deviceContext)
{
	ShaderGroup::SetupShaders(deviceContext);

	ID3D11SamplerState* sampler = SamplerStorage::Get()->GetSampler(m_samplerName);
	deviceContext->PSSetSamplers(0, 1, &sampler);
}

void DeferredLightShadowShaderGroup::SetupPerFrameBuffer(ID3D11DeviceContext * deviceContext, unsigned int nrOfResources, ID3D11ShaderResourceView ** resources, ID3D11ShaderResourceView * depthTexture, Vector3f lightPosition, const DirectX::XMMATRIX & lightViewMatrix, const DirectX::XMMATRIX & lightProjectionMatrix, float lightIntensity)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	PS_PerFrameBuffer* frameDataPS;
	HRESULT result;

	// Pixel Shader ===========================================================================
	// Mapping and updating buffer
	result = deviceContext->Map(
		m_psPerFrameBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource
		);
	if (FAILED(result))
	{
		return;
	}

	frameDataPS = (PS_PerFrameBuffer*)mappedResource.pData;
	frameDataPS->lightView = lightViewMatrix;
	frameDataPS->lightProj = lightProjectionMatrix;
	frameDataPS->lightPosition = lightPosition;
	frameDataPS->lightIntensity = lightIntensity;

	deviceContext->Unmap(m_psPerFrameBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &m_psPerFrameBuffer);

	deviceContext->PSSetShaderResources(0, nrOfResources, resources);
	deviceContext->PSSetShaderResources(nrOfResources, 1, &depthTexture);
}