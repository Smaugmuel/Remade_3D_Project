#ifndef DEPTH_SHADER_GROUP_HPP
#define DEPTH_SHADER_GROUP_HPP
#include "ShaderGroup.hpp"

#include <DirectXMath.h>
#include <string>

struct ID3D11Buffer;

class DepthShaderGroup : public ShaderGroup
{
	struct VS_PerFrameBuffer
	{
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
	};
	struct VS_PerObjectBuffer
	{
		DirectX::XMMATRIX world;
	};

public:
	DepthShaderGroup();
	~DepthShaderGroup();

	bool Initialize(ID3D11Device* device) override;

	void SetupShaders(ID3D11DeviceContext* deviceContext) override;
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & viewMatrix, const DirectX::XMMATRIX & projectionMatrix);
	void SetupPerObjectBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & worldMatrix);

private:
	ID3D11Buffer* m_vsPerFrameBuffer;
	ID3D11Buffer* m_vsPerObjectBuffer;
};

#endif