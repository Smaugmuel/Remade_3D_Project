#ifndef DEFERRED_LIGHT_SHADOW_SHADER_GROUP_HPP
#define DEFERRED_LIGHT_SHADOW_SHADER_GROUP_HPP
#include "Vector3.hpp"
#include <DirectXMath.h>
#include <string>

struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11ShaderResourceView;

class DeferredLightShadowShaderGroup
{
	struct PS_PerFrameBuffer
	{
		Vector3f lightPosition;
		float lightIntensity;
		DirectX::XMMATRIX lightView;
		DirectX::XMMATRIX lightProj;
	};

public:
	DeferredLightShadowShaderGroup();
	~DeferredLightShadowShaderGroup();

	bool Initialize(ID3D11Device* device);

	void SetupShaders(ID3D11DeviceContext* deviceContext);
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext, unsigned int nrOfResources, ID3D11ShaderResourceView** resources, ID3D11ShaderResourceView* depthTexture, Vector3f lightPosition, const DirectX::XMMATRIX & lightViewMatrix, const DirectX::XMMATRIX & lightProjectionMatrix, float lightIntensity);

private:
	std::string m_vertexShaderName;
	std::string m_pixelShaderName;

	std::string m_samplerName;

	ID3D11Buffer* m_psPerFrameBuffer;
};

#endif