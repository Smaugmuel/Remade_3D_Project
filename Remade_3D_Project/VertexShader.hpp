#ifndef VERTEX_SHADER_HPP
#define VERTEX_SHADER_HPP
#include "Shader.hpp"
#include <DirectXMath.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11VertexShader;
struct ID3D11InputLayout;
struct ID3D10Blob;
struct ID3D11Buffer;

class Object;
class Camera;

class VertexShader : public Shader
{
	struct PerObjectConstantBuffer
	{
		DirectX::XMMATRIX world;
	};
	struct PerFrameConstantBuffer
	{
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

public:
	VertexShader();
	~VertexShader();

	bool Initialize(ID3D11Device* device, const char* shaderType) override;

	void SetPerFrameConstantBuffer(ID3D11DeviceContext* deviceContext, Camera* camera);
	void SetPerObjectConstantBuffer(ID3D11DeviceContext* deviceContext, Object* object);

	ID3D11VertexShader* GetShader() const;
	ID3D11InputLayout* GetInputLayout() const;
	ID3D11Buffer* GetPerFrameConstantBuffer() const;
	ID3D11Buffer* GetPerObjectConstantBuffer() const;


private:
	bool InitializeInputLayout(ID3D11Device* device, ID3D10Blob* vertexShaderBuffer, const char* shaderType);
	bool InitializeConstantBuffers(ID3D11Device* device);

	ID3D11InputLayout* m_inputLayout;
	ID3D11VertexShader* m_vertexShader;
	ID3D11Buffer* m_perObjectConstantBuffer;
	ID3D11Buffer* m_perFrameConstantBuffer;
};

#endif