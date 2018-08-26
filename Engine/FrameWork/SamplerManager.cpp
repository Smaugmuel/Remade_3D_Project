#include "SamplerManager.hpp"
#include <d3d11.h>

SamplerManager::SamplerManager() : m_device(nullptr), m_deviceContext(nullptr)
{
	for (unsigned int i = 0; i < NR_OF_SAMPLER_STATES; i++)
	{
		m_samplerStates[i] = nullptr;
	}
}

SamplerManager::~SamplerManager()
{
	for (unsigned int i = 0; i < NR_OF_SAMPLER_STATES; i++)
	{
		if (m_samplerStates[i])
		{
			m_samplerStates[i]->Release();
		}
	}
}

bool SamplerManager::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext)
{
	m_device = device;
	m_deviceContext = deviceContext;

	D3D11_SAMPLER_DESC desc;
	/*desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;*/
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.MipLODBias = 0.0f;
	desc.MaxAnisotropy = 1;
	desc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
	desc.BorderColor[0] = 0;
	desc.BorderColor[1] = 0;
	desc.BorderColor[2] = 0;
	desc.BorderColor[3] = 0;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	if (FAILED(device->CreateSamplerState(&desc, &m_samplerStates[static_cast<unsigned int>(SamplerType::CLAMP)])))
		return false;

	desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;

	if (FAILED(device->CreateSamplerState(&desc, &m_samplerStates[static_cast<unsigned int>(SamplerType::WRAP)])))
		return false;

	return true;
}

bool SamplerManager::SetSamplerStateToPixelShader(SamplerType type, int slot)
{
	m_deviceContext->PSSetSamplers(slot, 1, &m_samplerStates[static_cast<unsigned int>(type)]);
	return true;
}
