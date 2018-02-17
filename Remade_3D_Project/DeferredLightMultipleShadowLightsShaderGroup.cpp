#include "DeferredLightMultipleShadowLightsShaderGroup.hpp"
#include <d3d11.h>

#include "SamplerStorage.hpp"

#include "ConstantBufferStorage.hpp"

DeferredLightMultipleShadowLightsShaderGroup::DeferredLightMultipleShadowLightsShaderGroup()
{
}

DeferredLightMultipleShadowLightsShaderGroup::~DeferredLightMultipleShadowLightsShaderGroup()
{
}

bool DeferredLightMultipleShadowLightsShaderGroup::Initialize(ID3D11Device * device)
{
	D3D11_BUFFER_DESC ps_perFrameDesc;

	m_vertexShaderName = "VS_PosUV.hlsl";
	m_pixelShaderName = "PS_D_LightMultipleShadowLights.hlsl";

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

void DeferredLightMultipleShadowLightsShaderGroup::SetupShaders(ID3D11DeviceContext * deviceContext)
{
	ShaderGroup::SetupShaders(deviceContext);

	ID3D11SamplerState* sampler = SamplerStorage::Get()->GetSampler(m_samplerName);
	deviceContext->PSSetSamplers(0, 1, &sampler);
}

void DeferredLightMultipleShadowLightsShaderGroup::SetupPerFrameBuffer(ID3D11DeviceContext * deviceContext, unsigned int nrOfResources, ID3D11ShaderResourceView ** resources, unsigned int nrOfLights, ID3D11ShaderResourceView * depthTextures[MAX_NR_OF_LIGHTS], Vector3f lightPositions[MAX_NR_OF_LIGHTS], DirectX::XMMATRIX lightViewMatrices[MAX_NR_OF_LIGHTS], DirectX::XMMATRIX lightProjectionMatrices[MAX_NR_OF_LIGHTS], float lightIntensities[MAX_NR_OF_LIGHTS])
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
	for (unsigned int i = 0; i < nrOfLights; i++)
	{
		frameDataPS->lights[i].position = lightPositions[i];
		frameDataPS->lights[i].intensity = lightIntensities[i];
		frameDataPS->lights[i].view = lightViewMatrices[i];
		frameDataPS->lights[i].proj = lightProjectionMatrices[i];
	}
	frameDataPS->nrOfLights = nrOfLights;
	deviceContext->Unmap(m_psPerFrameBuffer, 0);

	deviceContext->PSSetConstantBuffers(0, 1, &m_psPerFrameBuffer);


	/*for (unsigned int i = 0; i < nrOfLights; i++)
	{
		m_ps_per_frame_buffer->m_data.lights[i].position = lightPositions[i];
		m_ps_per_frame_buffer->m_data.lights[i].intensity = lightIntensities[i];
		m_ps_per_frame_buffer->m_data.lights[i].view = lightViewMatrices[i];
		m_ps_per_frame_buffer->m_data.lights[i].proj = lightProjectionMatrices[i];
	}
	m_ps_per_frame_buffer->m_data.nrOfLights = nrOfLights;

	m_ps_per_frame_buffer->MapData(deviceContext);
	deviceContext->PSSetConstantBuffers(0, 1, &m_ps_per_frame_buffer->m_buffer);*/



	deviceContext->PSSetShaderResources(0, nrOfResources, resources);
	deviceContext->PSSetShaderResources(nrOfResources, nrOfLights, depthTextures);
}
