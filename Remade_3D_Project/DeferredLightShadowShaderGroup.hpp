#ifndef DEFERRED_LIGHT_SHADOW_SHADER_GROUP_HPP
#define DEFERRED_LIGHT_SHADOW_SHADER_GROUP_HPP
#include "ShaderGroup.hpp"

#include "Vector3.hpp"
#include <DirectXMath.h>
#include <string>

struct ID3D11ShaderResourceView;

class DeferredLightShadowShaderGroup : public ShaderGroup
{
public:
	DeferredLightShadowShaderGroup();
	~DeferredLightShadowShaderGroup();

	bool Initialize(ID3D11Device* device) override;

	void SetupShaders(ID3D11DeviceContext* deviceContext) override;
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext, unsigned int nrOfResources, ID3D11ShaderResourceView** resources, ID3D11ShaderResourceView* depthTexture);

private:
	std::string m_samplerName;
};

#endif