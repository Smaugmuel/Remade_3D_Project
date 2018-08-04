#ifndef SAMPLER_STORAGE_HPP
#define SAMPLER_STORAGE_HPP
#include "../Misc/Singleton.hpp"
#include <map>
#include <string>

struct ID3D11Device;
struct ID3D11SamplerState;

class SamplerStorage final : public Singleton<SamplerStorage>
{
	friend class Singleton<SamplerStorage>;
	
	typedef std::map<std::string, ID3D11SamplerState*> SamplerMap;
	
	SamplerStorage();
	~SamplerStorage();

public:
	bool Initialize(ID3D11Device* device);

	ID3D11SamplerState * GetSampler(const std::string& name) const;

	bool HasSampler(const std::string& name) const;

private:
	SamplerMap m_samplers;
};

#endif