#ifndef DEFERRED_TEXTURE_SHADER_GROUP_HPP
#define DEFERRED_TEXTURE_SHADER_GROUP_HPP
#include "ShaderGroup.hpp"

#include <DirectXMath.h>
#include "Vector3.hpp"
#include <string>

struct ID3D11Buffer;
struct ID3D11ShaderResourceView;

class DeferredTextureShaderGroup : public ShaderGroup
{
	struct VS_PerObjectBuffer
	{
		DirectX::XMMATRIX world;
	};
	struct VS_PerFrameBuffer
	{
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

public:
	DeferredTextureShaderGroup();
	~DeferredTextureShaderGroup();

	bool Initialize(ID3D11Device* device) override;

	void SetupShaders(ID3D11DeviceContext* deviceContext) override;
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& viewMatrix, const DirectX::XMMATRIX& projectionMatrix);
	void SetupPerObjectBuffer(ID3D11DeviceContext* deviceContext, /*const DirectX::XMMATRIX& worldMatrix,*/ ID3D11ShaderResourceView* texture);

private:
	std::string m_samplerName;

	//ID3D11Buffer** m_vsBuffers;
	ID3D11Buffer* m_vsPerFrameBuffer;
	ID3D11Buffer* m_vsPerObjectBuffer;
};

#endif