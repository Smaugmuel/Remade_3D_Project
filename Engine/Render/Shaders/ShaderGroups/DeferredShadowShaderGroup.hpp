#ifndef DEFERRED_SHADOW_SHADER_GROUP_HPP
#define DEFERRED_SHADOW_SHADER_GROUP_HPP
#include "../ShaderGroup.hpp"

#include <DirectXMath.h>
#include "../../../Math/Vector3.hpp"

struct ID3D11Buffer;

class DeferredShadowShaderGroup final : public ShaderGroup
{
	struct VS_PerFrameBuffer
	{
		DirectX::XMMATRIX lightView;
		DirectX::XMMATRIX lightProj;
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

	bool Initialize(ID3D11Device* device) final override;

	void SetupShaders(ID3D11DeviceContext* deviceContext) final override;
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & lightViewMatrix, const DirectX::XMMATRIX & lightProjectionMatrix);
	void SetupPerObjectBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & worldMatrix);

private:
	ID3D11Buffer* m_vsPerFrameBuffer;
	ID3D11Buffer* m_vsPerObjectBuffer;
};


#endif