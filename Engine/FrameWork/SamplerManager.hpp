#ifndef SAMPLER_MANAGER_HPP
#define SAMPLER_MANAGER_HPP

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11SamplerState;

enum class SamplerType
{
	WRAP, CLAMP
};

class SamplerManager final
{
public:
	SamplerManager();
	~SamplerManager();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	bool SetSamplerStateToPixelShader(SamplerType type, int slot);

private:
	static const unsigned int NR_OF_SAMPLER_STATES = static_cast<unsigned int>(SamplerType::CLAMP) + 1;

	ID3D11SamplerState* m_samplerStates[NR_OF_SAMPLER_STATES];
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
};

#endif