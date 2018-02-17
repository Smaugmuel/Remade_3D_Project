#ifndef SINGLE_COLOR_SHADER_GROUP_HPP
#define SINGLE_COLOR_SHADER_GROUP_HPP
#include "ShaderGroup.hpp"

#include <DirectXMath.h>
#include "Vector3.hpp"
#include <string>

struct ID3D11Buffer;

class SingleColorShaderGroup : public ShaderGroup
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
	struct PS_PerFrameBuffer
	{
		// Stored as a float4 in shader to prevent (?) automatic padding
		Vector3f lightPosition;
		float lightIntensity;
	};

public:
	SingleColorShaderGroup();
	~SingleColorShaderGroup();

	bool Initialize(ID3D11Device* device) override;

	void SetupShaders(ID3D11DeviceContext* deviceContext) override;
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & viewMatrix, const DirectX::XMMATRIX & projectionMatrix, Vector3f lightPosition, const DirectX::XMMATRIX & lightViewMatrix, const DirectX::XMMATRIX & lightProjectionMatrix, float lightIntensity);
	void SetupPerObjectBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & worldMatrix, Vector3f color);

private:
	//ID3D11Buffer** m_vsBuffers;
	ID3D11Buffer* m_vsPerObjectBuffer;
	ID3D11Buffer* m_vsPerFrameBuffer;
	ID3D11Buffer* m_psPerFrameBuffer;
};

#endif