#ifndef DEFERRED_LIGHT_MULTIPLE_LIGHTS_SHADER_GROUP_HPP
#define DEFERRED_LIGHT_MULTIPLE_LIGHTS_SHADER_GROUP_HPP
#include "ShaderGroup.hpp"

#include "Vector3.hpp"

struct ID3D11ShaderResourceView;

class DeferredLightMultipleLightsShaderGroup : public ShaderGroup
{
public:
	DeferredLightMultipleLightsShaderGroup();
	~DeferredLightMultipleLightsShaderGroup();

	bool Initialize(ID3D11Device* device) override;

	void SetupShaders(ID3D11DeviceContext* deviceContext) override;
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext, unsigned int nrOfResources, ID3D11ShaderResourceView** resources);

private:
	std::string m_samplerName;
};

#endif