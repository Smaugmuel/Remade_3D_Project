#ifndef DEFERRED_LIGHT_MULTIPLE_SHADOW_LIGHTS_SHADER_GROUP_HPP
#define DEFERRED_LIGHT_MULTIPLE_SHADOW_LIGHTS_SHADER_GROUP_HPP
#include "ShaderGroup.hpp"

#include "Vector3.hpp"
#include <DirectXMath.h>
#include "LightSettings.hpp"

struct ID3D11Buffer;
struct ID3D11ShaderResourceView;

class DeferredLightMultipleShadowLightsShaderGroup : public ShaderGroup
{
	struct PointLight
	{
		Vector3f position;
		float intensity;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
	};

	struct PS_PerFrameBuffer
	{
		PointLight lights[MAX_NR_OF_LIGHTS];
		unsigned int nrOfLights;
		int padding1;
		int padding2;
		int padding3;
	};

public:
	DeferredLightMultipleShadowLightsShaderGroup();
	~DeferredLightMultipleShadowLightsShaderGroup();

	bool Initialize(ID3D11Device* device) override;

	void SetupShaders(ID3D11DeviceContext* deviceContext) override;
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext, unsigned int nrOfResources, ID3D11ShaderResourceView** resources, ID3D11ShaderResourceView** depthTextures);

private:
	std::string m_samplerName;

	ID3D11Buffer* m_psPerFrameBuffer;
};

#endif