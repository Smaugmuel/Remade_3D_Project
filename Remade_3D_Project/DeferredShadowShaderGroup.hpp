#ifndef DEFERRED_SHADOW_SHADER_GROUP_HPP
#define DEFERRED_SHADOW_SHADER_GROUP_HPP
#include "ShaderGroup.hpp"

#include <DirectXMath.h>
#include "Vector3.hpp"

struct ID3D11Buffer;

class DeferredShadowShaderGroup : public ShaderGroup
{
	struct VS_PerFrameBuffer
	{
		//DirectX::XMMATRIX view;
		//DirectX::XMMATRIX projection;
		DirectX::XMMATRIX lightView;
		DirectX::XMMATRIX lightProj;
		//Vector3f lightPosition;
		//float padding;
	};
	struct VS_PerObjectBuffer
	{
		DirectX::XMMATRIX world;
		Vector3f color;
		float padding;
	};

public:
	DeferredShadowShaderGroup();
	~DeferredShadowShaderGroup();

	bool Initialize(ID3D11Device* device) override;

	void SetupShaders(ID3D11DeviceContext* deviceContext) override;
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & lightViewMatrix, const DirectX::XMMATRIX & lightProjectionMatrix);
	void SetupPerObjectBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & worldMatrix);

private:
	//ID3D11VertexShader* m_vs;
	//ID3D11PixelShader* m_ps;
	//ID3D11InputLayout* m_layout;
	//ID3D11SamplerState* m_sampleStateClamp;

	ID3D11Buffer* m_vsPerFrameBuffer;
	ID3D11Buffer* m_vsPerObjectBuffer;
};


#endif