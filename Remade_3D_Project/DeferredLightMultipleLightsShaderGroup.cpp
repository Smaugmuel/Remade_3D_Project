#include "DeferredLightMultipleLightsShaderGroup.hpp"
#include <d3d11.h>

#include "ShaderStorage.hpp"
#include "SamplerStorage.hpp"

//#include "ConstantBufferStorage.hpp"

DeferredLightMultipleLightsShaderGroup::DeferredLightMultipleLightsShaderGroup()
{
}

DeferredLightMultipleLightsShaderGroup::~DeferredLightMultipleLightsShaderGroup()
{
}

bool DeferredLightMultipleLightsShaderGroup::Initialize(ID3D11Device* device)
{
	D3D11_BUFFER_DESC ps_perFrameDesc;
	HRESULT result;

	m_vertexShaderName = "VS_PosUV.hlsl";
	m_pixelShaderName = "PS_D_LightMultipleLights.hlsl";
	m_samplerName = "PointClamp";

	if (!ShaderStorage::Get()->CreateVertexShader(device, m_vertexShaderName))
		return false;
	if (!ShaderStorage::Get()->CreatePixelShader(device, m_pixelShaderName))
		return false;

	/*m_ps_per_frame_buffer = new ConstantBuffer<PS_PerFrameBuffer>();
	if (!m_ps_per_frame_buffer->Initialize(device))
		return false;*/


	// Create per-frame constant buffer ===========================================================
	memset(&ps_perFrameDesc, 0, sizeof(ps_perFrameDesc));
	ps_perFrameDesc.Usage = D3D11_USAGE_DYNAMIC;
	ps_perFrameDesc.ByteWidth = sizeof(PS_PerFrameBuffer);
	ps_perFrameDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ps_perFrameDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ps_perFrameDesc.MiscFlags = 0;
	ps_perFrameDesc.StructureByteStride = 0;
	
	result = device->CreateBuffer(&ps_perFrameDesc, nullptr, &m_psPerFrameBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void DeferredLightMultipleLightsShaderGroup::SetupShaders(ID3D11DeviceContext * deviceContext)
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

void DeferredLightMultipleLightsShaderGroup::SetupPerFrameBuffer(ID3D11DeviceContext * deviceContext, unsigned int nrOfResources, ID3D11ShaderResourceView ** resources, Vector3f lightPositions[MAX_NR_OF_LIGHTS], float lightIntensitys[MAX_NR_OF_LIGHTS])
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
	for (unsigned int i = 0; i < MAX_NR_OF_LIGHTS; i++)
	{
		frameDataPS->lights[i].position = lightPositions[i];
		frameDataPS->lights[i].intensity = lightIntensitys[i];
	}

	deviceContext->Unmap(m_psPerFrameBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &m_psPerFrameBuffer);


	/*for (unsigned int i = 0; i < MAX_NR_OF_LIGHTS; i++)
	{
		m_ps_per_frame_buffer->m_data.lights[i].position = lightPositions[i];
		m_ps_per_frame_buffer->m_data.lights[i].intensity = lightIntensitys[i];
	}

	m_ps_per_frame_buffer->MapData(deviceContext);
	deviceContext->PSSetConstantBuffers(0, 1, &m_ps_per_frame_buffer->m_buffer);*/


	deviceContext->PSSetShaderResources(0, nrOfResources, resources);
}