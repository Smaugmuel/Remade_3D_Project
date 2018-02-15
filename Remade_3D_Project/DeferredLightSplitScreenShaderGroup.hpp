#ifndef DEFERRED_LIGHT_SPLIT_SCREEN_SHADER_GROUP_HPP
#define DEFERRED_LIGHT_SPLIT_SCREEN_SHADER_GROUP_HPP

#include "Vector3.hpp"
#include <DirectXMath.h>
#include <string>

struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11SamplerState;
struct ID3D11ShaderResourceView;

class Camera;

class DeferredLightSplitScreenShaderGroup
{
	struct PS_PerFrameBuffer
	{
		Vector3f lightPosition;
		float lightIntensity;
		DirectX::XMMATRIX lightView;
		DirectX::XMMATRIX lightProj;
	};

public:
	DeferredLightSplitScreenShaderGroup();
	~DeferredLightSplitScreenShaderGroup();

	bool Initialize(ID3D11Device* device);

	void SetupShaders(ID3D11DeviceContext* deviceContext);
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext, unsigned int nrOfResources, ID3D11ShaderResourceView** resources, ID3D11ShaderResourceView* depthTexture, Vector3f lightPosition, const DirectX::XMMATRIX & lightViewMatrix, const DirectX::XMMATRIX & lightProjectionMatrix, float lightIntensity);

private:
	std::string m_vertexShaderName;
	std::string m_pixelShaderName;

	ID3D11Buffer* m_psPerFrameBuffer;

	ID3D11SamplerState* m_samplerState;
};

#endif