#ifndef DEFERRED_SINGLE_COLOR_SHADER_GROUP_HPP
#define DEFERRED_SINGLE_COLOR_SHADER_GROUP_HPP
#include "ShaderGroup.hpp"

#include <DirectXMath.h>
#include "Vector3.hpp"
#include <string>

struct ID3D11Buffer;

class DeferredSingleColorShaderGroup : public ShaderGroup
{
	struct VS_PerObjectBuffer
	{
		DirectX::XMMATRIX world;
		Vector3f color;
		float padding;
	};
	struct VS_PerFrameBuffer
	{
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

public:
	DeferredSingleColorShaderGroup();
	~DeferredSingleColorShaderGroup();

	bool Initialize(ID3D11Device* device) override;

	void SetupShaders(ID3D11DeviceContext* deviceContext) override;
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & viewMatrix, const DirectX::XMMATRIX & projectionMatrix);
	void SetupPerObjectBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & worldMatrix, Vector3f color);

private:
	//ID3D11Buffer** m_vsBuffers;
	ID3D11Buffer* m_vsPerFrameBuffer;
	ID3D11Buffer* m_vsPerObjectBuffer;
};

#endif