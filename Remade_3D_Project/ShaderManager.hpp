#ifndef SHADER_MANAGER_HPP
#define SHADER_MANAGER_HPP
#include "Singleton.hpp"

#include <memory>
//#include <string>
#include "Vector3.hpp"
#include <DirectXMath.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11ShaderResourceView;

class SingleColorShaderGroup;
class TextureShaderGroup;
class DepthShaderGroup;
class HUDShaderGroup;
class DeferredSingleColorShaderGroup;
class DeferredTextureShaderGroup;
class DeferredShadowShaderGroup;
class DeferredLightShaderGroup;

enum ShaderType
{
	SINGLE_COLOR,
	D_SINGLE_COLOR,
	D_LIGHT,
	DEPTH,
	D_SHADOW,
	TEXTURE,
	D_TEXTURE,
	HUD
};

class ShaderManager : public Singleton<ShaderManager>
{
	friend class Singleton<ShaderManager>;
	ShaderManager();
	~ShaderManager();

public:
	bool Initialize(ID3D11Device* device);

	void SetShaderType(ID3D11DeviceContext* deviceContext, const ShaderType& shaderType);
	
	void SetPerFrameSingleColorConstantBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & viewMatrix, const DirectX::XMMATRIX & projectionMatrix, const DirectX::XMMATRIX & lightViewMatrix, const DirectX::XMMATRIX & lightProjectionMatrix, Vector3f lightPosition, float lightIntensity);
	void SetPerFrameTextureConstantBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & viewMatrix, const DirectX::XMMATRIX & projectionMatrix, Vector3f lightPosition, float lightIntensity);
	void SetPerFrameDepthConstantBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & viewMatrix, const DirectX::XMMATRIX & projectionMatrix);
	void SetPerFrameDeferredSingleColorConstantBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & viewMatrix, const DirectX::XMMATRIX & projectionMatrix);
	void SetPerFrameDeferredTextureConstantBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & viewMatrix, const DirectX::XMMATRIX & projectionMatrix);
	void SetPerFrameDeferredLightConstantBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & lightViewMatrix, const DirectX::XMMATRIX & lightProjectionMatrix, ID3D11ShaderResourceView* depthTexture, Vector3f lightPosition, float lightIntensity);
	void SetPerFrameDeferredShadowConstantBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & lightViewMatrix, const DirectX::XMMATRIX & lightProjectionMatrix);

	void SetPerObjectSingleColorConstantBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & worldMatrix, Vector3f color);
	void SetPerObjectTextureConstantBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& worldMatrix, ID3D11ShaderResourceView* texture);
	void SetPerObjectDepthConstantBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & worldMatrix);
	void SetPerObjectHUDConstantBuffer(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture);
	void SetPerObjectDeferredSingleColorConstantBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & worldMatrix, Vector3f color);
	void SetPerObjectDeferredTextureConstantBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& worldMatrix, ID3D11ShaderResourceView* texture);
	void SetPerObjectDeferredShadowConstantBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & worldMatrix);

private:
	std::unique_ptr<SingleColorShaderGroup> m_colorShaders;
	std::unique_ptr<TextureShaderGroup> m_textureShaders;
	std::unique_ptr<DepthShaderGroup> m_depthShaders;
	std::unique_ptr<HUDShaderGroup> m_HUDShaders;
	std::unique_ptr<DeferredSingleColorShaderGroup> m_d_colorShaders;
	std::unique_ptr<DeferredTextureShaderGroup> m_d_textureShaders;
	std::unique_ptr<DeferredShadowShaderGroup> m_s_shaders;
	std::unique_ptr<DeferredLightShaderGroup> m_d_lightShaders;


	ShaderType m_currentShaderType;
};

#endif