#ifndef DEPTH_SHADER_GROUP_HPP
#define DEPTH_SHADER_GROUP_HPP
#include <DirectXMath.h>

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;

class Camera;
class Object;

class DepthShaderGroup
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

	bool Initialize(ID3D11Device* device);

	void SetupShaders(ID3D11DeviceContext* deviceContext);
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & viewMatrix, const DirectX::XMMATRIX & projectionMatrix);
	void SetupPerObjectBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & worldMatrix);

private:
	ID3D11VertexShader* m_vs;
	ID3D11PixelShader* m_ps;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_vsPerFrameBuffer;
	ID3D11Buffer* m_vsPerObjectBuffer;
};

#endif