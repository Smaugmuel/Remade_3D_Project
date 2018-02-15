#ifndef DEFERRED_LIGHT_MULTIPLE_SHADOW_LIGHTS_SHADER_GROUP_HPP
#define DEFERRED_LIGHT_MULTIPLE_SHADOW_LIGHTS_SHADER_GROUP_HPP

#include "Vector3.hpp"
#include <DirectXMath.h>
#include "SystemInformation.hpp"
#include <string>

struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11SamplerState;
struct ID3D11ShaderResourceView;

class DeferredLightMultipleShadowLightsShaderGroup
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
		int nrOfLights;
		int padding1;
		int padding2;
		int padding3;
	};

public:
	DeferredLightMultipleShadowLightsShaderGroup();
	~DeferredLightMultipleShadowLightsShaderGroup();

	bool Initialize(ID3D11Device* device);

	void SetupShaders(ID3D11DeviceContext* deviceContext);
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext, unsigned int nrOfResources, ID3D11ShaderResourceView** resources, unsigned int nrOfLights, ID3D11ShaderResourceView* depthTextures[MAX_NR_OF_LIGHTS], Vector3f lightPositions[MAX_NR_OF_LIGHTS], DirectX::XMMATRIX lightViewMatrices[MAX_NR_OF_LIGHTS], DirectX::XMMATRIX lightProjectionMatrices[MAX_NR_OF_LIGHTS], float lightIntensities[MAX_NR_OF_LIGHTS]);

private:
	std::string m_vertexShaderName;
	std::string m_pixelShaderName;

	ID3D11Buffer* m_psPerFrameBuffer;

	ID3D11SamplerState* m_samplerState;
};

#endif