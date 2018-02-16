#include "SamplerStorage.hpp"
#include <d3d11.h>

SamplerStorage* Singleton<SamplerStorage>::s_instance = nullptr;

SamplerStorage::SamplerStorage()
{
}

SamplerStorage::~SamplerStorage()
{
}

bool SamplerStorage::Initialize(ID3D11Device * device)
{
	D3D11_SAMPLER_DESC desc;
	/*desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;*/
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.MipLODBias = 0.0f;
	desc.MaxAnisotropy = 1;
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	desc.BorderColor[0] = 0;
	desc.BorderColor[1] = 0;
	desc.BorderColor[2] = 0;
	desc.BorderColor[3] = 0;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	if (FAILED(device->CreateSamplerState(&desc, &m_samplers["PointClamp"])))
	{
		return false;
	}


	return true;
}

ID3D11SamplerState * SamplerStorage::GetSampler(const std::string & name) const
{
	if (!HasSampler(name))
		return nullptr;

	return m_samplers.at(name);
}

bool SamplerStorage::HasSampler(const std::string & name) const
{
	return m_samplers.find(name) != m_samplers.end();
}
