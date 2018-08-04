#ifndef DEFERRED_LIGHT_SHADOW_SHADER_GROUP_HPP
#define DEFERRED_LIGHT_SHADOW_SHADER_GROUP_HPP
#include "../ShaderGroup.hpp"
#include "../../../Math/Vector3.hpp"

#include <DirectXMath.h>
#include <string>

struct ID3D11ShaderResourceView;

class DeferredLightShadowShaderGroup final : public ShaderGroup
{
public:
	DeferredLightShadowShaderGroup();
	~DeferredLightShadowShaderGroup();

	bool Initialize(ID3D11Device* device) final override;

	void SetupShaders(ID3D11DeviceContext* deviceContext) final override;
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext, unsigned int nrOfResources, ID3D11ShaderResourceView** resources, ID3D11ShaderResourceView* depthTexture);

private:
	std::string m_samplerName;
};

#endif