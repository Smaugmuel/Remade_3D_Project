#ifndef TEXTURE_SHADER_GROUP_HPP
#define TEXTURE_SHADER_GROUP_HPP
#include <DirectXMath.h>
#include "Vector2.hpp"
#include "Vector3.hpp"
#include <string>

struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11SamplerState;
struct ID3D11ShaderResourceView;

class TextureShaderGroup
{
	struct VS_PerObjectBuffer
	{
		DirectX::XMMATRIX world;
	};
	struct VS_PerFrameBuffer
	{
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
		Vector3f lightPosition;
		float padding;
	};
	//struct PS_PerFrameBuffer
	//{
	//	// Stored as a float4 in shader to prevent (?) automatic padding
	//	Vector3f lightPosition;
	//	float lightIntensity;
	//};

public:
	TextureShaderGroup();
	~TextureShaderGroup();

	bool Initialize(ID3D11Device* device);

	void SetupShaders(ID3D11DeviceContext* deviceContext);
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& viewMatrix, const DirectX::XMMATRIX& projectionMatrix, Vector3f lightPosition, float lightIntensity);
	void SetupPerObjectBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& worldMatrix, ID3D11ShaderResourceView* texture);

private:
	std::string m_vertexShaderName;
	std::string m_pixelShaderName;

	std::string m_samplerName;

	ID3D11Buffer* m_vsPerObjectBuffer;
	ID3D11Buffer* m_vsPerFrameBuffer;
	//ID3D11Buffer* m_psPerFrameBuffer;
};

#endif