#ifndef DEFERRED_LIGHT_MULTIPLE_LIGHTS_SHADOW_SHADER_GROUP_HPP
#define DEFERRED_LIGHT_MULTIPLE_LIGHTS_SHADOW_SHADER_GROUP_HPP

#include "Vector3.hpp"
#include <DirectXMath.h>
#include "SystemInformation.hpp"

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11SamplerState;
struct ID3D11ShaderResourceView;

class Camera;


class DeferredLightMultipleLightsShadowShaderGroup
{
	struct Light
	{
		Vector3f position;
		float intensity;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
	};

	struct PS_PerFrameBuffer
	{
		Light lights[MAX_NR_OF_LIGHTS];
		int nrOfLights;
		int padding1;
		int padding2;
		int padding3;
	};

public:
	DeferredLightMultipleLightsShadowShaderGroup();
	~DeferredLightMultipleLightsShadowShaderGroup();

	bool Initialize(ID3D11Device* device);

	void SetupShaders(ID3D11DeviceContext* deviceContext);
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext, unsigned int nrOfResources, ID3D11ShaderResourceView** resources, ID3D11ShaderResourceView* depthTextures[MAX_NR_OF_LIGHTS], DirectX::XMMATRIX lightViewMatrices[MAX_NR_OF_LIGHTS], DirectX::XMMATRIX lightProjectionMatrices[MAX_NR_OF_LIGHTS], Vector3f lightPositions[MAX_NR_OF_LIGHTS], float lightIntensitys[MAX_NR_OF_LIGHTS], int nrOfLights);

private:
	ID3D11VertexShader* m_vs;
	ID3D11PixelShader* m_ps;

	ID3D11InputLayout* m_layout;

	ID3D11Buffer* m_psPerFrameBuffer;

	ID3D11SamplerState* m_samplerState;
};

#endif