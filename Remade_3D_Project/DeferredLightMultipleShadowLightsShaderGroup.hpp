#ifndef DEFERRED_LIGHT_MULTIPLE_SHADOW_LIGHTS_SHADER_GROUP_HPP
#define DEFERRED_LIGHT_MULTIPLE_SHADOW_LIGHTS_SHADER_GROUP_HPP
#include "ShaderGroup.hpp"

#include "Vector3.hpp"
#include <DirectXMath.h>
#include "SystemInformation.hpp"

struct ID3D11Buffer;
struct ID3D11ShaderResourceView;

//template<typename BufferData> struct ConstantBuffer;

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
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext, unsigned int nrOfResources, ID3D11ShaderResourceView** resources, unsigned int nrOfLights, ID3D11ShaderResourceView* depthTextures[MAX_NR_OF_LIGHTS], Vector3f lightPositions[MAX_NR_OF_LIGHTS], DirectX::XMMATRIX lightViewMatrices[MAX_NR_OF_LIGHTS], DirectX::XMMATRIX lightProjectionMatrices[MAX_NR_OF_LIGHTS], float lightIntensities[MAX_NR_OF_LIGHTS]);

private:
	std::string m_samplerName;

	//ConstantBuffer<PS_PerFrameBuffer>* m_ps_per_frame_buffer;

	ID3D11Buffer* m_psPerFrameBuffer;
};

#endif