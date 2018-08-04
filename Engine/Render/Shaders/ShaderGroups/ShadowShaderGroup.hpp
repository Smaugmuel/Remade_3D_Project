#ifndef SHADOW_SHADER_GROUP_HPP
#define SHADOW_SHADER_GROUP_HPP
#include <DirectXMath.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11SamplerState;
struct ID3D11Buffer;
struct ID3D11ShaderResourceView;

class Camera;
class Object;

class ShadowShaderGroup
{
	struct VS_PerFrame1
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
		DirectX::XMMATRIX lightView;
		DirectX::XMMATRIX lightProj;
	};

	struct VS_PerFrame2
	{
		DirectX::XMFLOAT3 lightPosition;
		float padding;
	};

	struct PS_PerFrame
	{
		DirectX::XMFLOAT4 ambientColor;
		DirectX::XMFLOAT4 diffuseColor;
	};


public:
	ShadowShaderGroup();
	~ShadowShaderGroup();

	bool Initialize(ID3D11Device* device);

	void SetupShader(ID3D11DeviceContext* deviceContext);
	void SetupPerFrameBuffer(ID3D11DeviceContext * deviceContext, Object* object, Camera* camera, Camera* lightCamera, ID3D11ShaderResourceView* depthTexture);

private:
	ID3D11VertexShader* m_vs;
	ID3D11PixelShader* m_ps;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_samplerStateClamp;
	ID3D11Buffer* m_vsPerFrameBuffer1;
	ID3D11Buffer* m_vsPerFrameBuffer2;
	ID3D11Buffer* m_psPerFrameBuffer;
};

#endif