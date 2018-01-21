#ifndef DEFERRED_TEXTURE_SHADER_GROUP_HPP
#define DEFERRED_TEXTURE_SHADER_GROUP_HPP
#include <DirectXMath.h>
#include "Vector3.hpp"

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11SamplerState;
struct ID3D11ShaderResourceView;

class Camera;
class Object;

class DeferredTextureShaderGroup
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

	bool Initialize(ID3D11Device* device);

	void SetupShaders(ID3D11DeviceContext* deviceContext);
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& viewMatrix, const DirectX::XMMATRIX& projectionMatrix);
	void SetupPerObjectBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& worldMatrix, ID3D11ShaderResourceView* texture);

private:
	ID3D11VertexShader* m_vs;
	ID3D11PixelShader* m_ps;

	ID3D11InputLayout* m_layout;

	//ID3D11Buffer** m_vsBuffers;
	ID3D11Buffer* m_vsPerFrameBuffer;
	ID3D11Buffer* m_vsPerObjectBuffer;

	ID3D11SamplerState* m_samplerState;
};

#endif