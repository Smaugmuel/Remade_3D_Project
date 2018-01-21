#ifndef DEFERRED_LIGHT_SHADER_GROUP_HPP
#define DEFERRED_LIGHT_SHADER_GROUP_HPP

#include "Vector3.hpp"
#include <DirectXMath.h>

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11SamplerState;
struct ID3D11ShaderResourceView;

class Camera;

class DeferredLightShaderGroup
{
	struct PS_PerFrameBuffer
	{
		Vector3f lightPosition;
		float lightIntensity;
		DirectX::XMMATRIX lightView;
		DirectX::XMMATRIX lightProj;
	};

public:
	DeferredLightShaderGroup();
	~DeferredLightShaderGroup();

	bool Initialize(ID3D11Device* device);

	void SetupShaders(ID3D11DeviceContext* deviceContext);
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext, unsigned int nrOfResources, ID3D11ShaderResourceView** resources, ID3D11ShaderResourceView* depthTexture, Vector3f lightPosition, const DirectX::XMMATRIX & lightViewMatrix, const DirectX::XMMATRIX & lightProjectionMatrix, float lightIntensity);

private:
	ID3D11VertexShader* m_vs;
	ID3D11PixelShader* m_ps;

	ID3D11InputLayout* m_layout;

	ID3D11Buffer* m_psPerFrameBuffer;

	ID3D11SamplerState* m_samplerState;
};

#endif