#ifndef DEFERRED_LIGHT_MULTIPLE_LIGHTS_SHADER_GROUP_HPP
#define DEFERRED_LIGHT_MULTIPLE_LIGHTS_SHADER_GROUP_HPP
#include "../ShaderGroup.hpp"

//#include "../../../Math/Vector3.hpp"

struct ID3D11ShaderResourceView;

class DeferredLightMultipleLightsShaderGroup final : public ShaderGroup
{
public:
	DeferredLightMultipleLightsShaderGroup();
	~DeferredLightMultipleLightsShaderGroup();

	bool Initialize(ID3D11Device* device) final override;

	void SetupShaders(ID3D11DeviceContext* deviceContext) final override;
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext, unsigned int nrOfResources, ID3D11ShaderResourceView** resources);

private:
	std::string m_samplerName;
};

#endif