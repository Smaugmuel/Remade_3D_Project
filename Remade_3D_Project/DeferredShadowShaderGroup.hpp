#ifndef DEFERRED_SHADOW_SHADER_GROUP_HPP
#define DEFERRED_SHADOW_SHADER_GROUP_HPP
#include <DirectXMath.h>
#include "Vector3.hpp"

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

class DeferredShadowShaderGroup
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

	bool Initialize(ID3D11Device* device);

	void SetupShaders(ID3D11DeviceContext* deviceContext);
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext, Camera* lightCamera);
	void SetupPerObjectBuffer(ID3D11DeviceContext* deviceContext, Object* object);

private:
	//ID3D11VertexShader* m_vs;
	//ID3D11PixelShader* m_ps;
	//ID3D11InputLayout* m_layout;
	//ID3D11SamplerState* m_sampleStateClamp;

	ID3D11Buffer* m_vsPerFrameBuffer;
	ID3D11Buffer* m_vsPerObjectBuffer;
};


#endif