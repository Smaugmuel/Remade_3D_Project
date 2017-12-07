#ifndef DEFERRED_SINGLE_COLOR_SHADER_GROUP_HPP
#define DEFERRED_SINGLE_COLOR_SHADER_GROUP_HPP
#include <DirectXMath.h>
#include "Vector3.hpp"

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;

class Camera;
class Object;

class DeferredSingleColorShaderGroup
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

	bool Initialize(ID3D11Device* device);

	void SetupShaders(ID3D11DeviceContext* deviceContext);
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext, Camera* camera);
	void SetupPerObjectBuffer(ID3D11DeviceContext* deviceContext, Object* object);

private:
	ID3D11VertexShader* m_vs;
	ID3D11PixelShader* m_ps;

	ID3D11InputLayout* m_layout;

	//ID3D11Buffer** m_vsBuffers;
	ID3D11Buffer* m_vsPerFrameBuffer;
	ID3D11Buffer* m_vsPerObjectBuffer;
};

#endif