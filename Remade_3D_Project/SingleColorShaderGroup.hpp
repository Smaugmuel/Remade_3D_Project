#ifndef SINGLE_COLOR_SHADER_GROUP_HPP
#define SINGLE_COLOR_SHADER_GROUP_HPP
#include <DirectXMath.h>
#include "Vector3.hpp"
#include <string>

//struct ID3D11VertexShader;
//struct ID3D11PixelShader;
//struct ID3D11InputLayout;
struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;

class Camera;
class Object;

class SingleColorShaderGroup
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

	bool Initialize(ID3D11Device* device);

	void SetupShaders(ID3D11DeviceContext* deviceContext);
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & viewMatrix, const DirectX::XMMATRIX & projectionMatrix, Vector3f lightPosition, const DirectX::XMMATRIX & lightViewMatrix, const DirectX::XMMATRIX & lightProjectionMatrix, float lightIntensity);
	void SetupPerObjectBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & worldMatrix, Vector3f color);

private:
	std::string m_vertexShaderName;
	std::string m_pixelShaderName;
	
	/*ID3D11VertexShader* m_vs;
	ID3D11PixelShader* m_ps;
	ID3D11InputLayout* m_layout;*/

	//ID3D11Buffer** m_vsBuffers;
	ID3D11Buffer* m_vsPerObjectBuffer;
	ID3D11Buffer* m_vsPerFrameBuffer;
	ID3D11Buffer* m_psPerFrameBuffer;
};

#endif