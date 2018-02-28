#ifndef SHADER_MANAGER_HPP
#define SHADER_MANAGER_HPP
#include "Singleton.hpp"

#include <memory>
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
class DeferredLightShadowShaderGroup;
class DeferredLightSplitScreenShaderGroup;
class DeferredLightMultipleLightsShaderGroup;
class DeferredLightMultipleShadowLightsShaderGroup;
class DeferredTextureChunkShaderGroup;

enum ShaderType
{
	SINGLE_COLOR,
	D_SINGLE_COLOR,
	D_LIGHT,
	DEPTH,
	D_SHADOW,
	TEXTURE,
	D_TEXTURE,
	HUD,
	D_SPLIT,
	D_MULTIPLE,
	D_MULTIPLE_SHADOWS,
	D_TEXTURE_CHUNK
};


class ShaderManager : public Singleton<ShaderManager>
{
	friend class Singleton<ShaderManager>;
	ShaderManager();
	~ShaderManager();

public:
	bool Initialize(ID3D11Device* device);

	void SetShaderType(ID3D11DeviceContext* deviceContext, const ShaderType& shaderType);
	
	void SetPerFrameSingleColorConstantBuffer(ID3D11DeviceContext* deviceContext, Vector3f lightPosition, float lightIntensity);
	void SetPerFrameTextureConstantBuffer(ID3D11DeviceContext* deviceContext, Vector3f lightPosition, float lightIntensity);
	void SetPerFrameDepthConstantBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & viewMatrix, const DirectX::XMMATRIX & projectionMatrix);
	void SetPerFrameDeferredSingleColorConstantBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & viewMatrix, const DirectX::XMMATRIX & projectionMatrix);
	void SetPerFrameDeferredTextureConstantBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & viewMatrix, const DirectX::XMMATRIX & projectionMatrix);
	void SetPerFrameDeferredShadowConstantBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & lightViewMatrix, const DirectX::XMMATRIX & lightProjectionMatrix);
	void SetPerFrameDeferredLightShadowConstantBuffer(ID3D11DeviceContext* deviceContext, unsigned int nrOfDeferredBuffers, ID3D11ShaderResourceView** deferredShaderResourceViews, ID3D11ShaderResourceView* depthTexture);
	void SetPerFrameDeferredLightSplitScreenConstantBuffer(ID3D11DeviceContext* deviceContext, unsigned int nrOfDeferredBuffers, ID3D11ShaderResourceView** deferredShaderResourceViews, ID3D11ShaderResourceView* depthTexture);
	void SetPerFrameDeferredLightMultipleLightsConstantBuffer(ID3D11DeviceContext* deviceContext, unsigned int nrOfDeferredBuffers, ID3D11ShaderResourceView** deferredShaderResourceViews);
	void SetPerFrameDeferredLightMultipleShadowLightsConstantBuffer(ID3D11DeviceContext* deviceContext, unsigned int nrOfResources, ID3D11ShaderResourceView** resources, ID3D11ShaderResourceView** depthTextures);

	void SetPerObjectSingleColorConstantBuffer(ID3D11DeviceContext* deviceContext);
	void SetPerObjectTextureConstantBuffer(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture);
	void SetPerObjectDepthConstantBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & worldMatrix);
	void SetPerObjectHUDConstantBuffer(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture);
	void SetPerObjectDeferredSingleColorConstantBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & worldMatrix, Vector3f color);
	void SetPerObjectDeferredTextureConstantBuffer(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture);
	void SetPerObjectDeferredShadowConstantBuffer(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX & worldMatrix);
	void SetPerObjectDeferredTextureChunkShaderGroup(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture);

private:
	std::unique_ptr<SingleColorShaderGroup> m_colorShaders;
	std::unique_ptr<TextureShaderGroup> m_textureShaders;
	std::unique_ptr<DepthShaderGroup> m_depthShaders;
	std::unique_ptr<HUDShaderGroup> m_HUDShaders;
	std::unique_ptr<DeferredSingleColorShaderGroup> m_d_colorShaders;
	std::unique_ptr<DeferredTextureShaderGroup> m_d_textureShaders;
	std::unique_ptr<DeferredShadowShaderGroup> m_d_s_shaders;
	std::unique_ptr<DeferredLightShadowShaderGroup> m_d_lightShadowShaders;
	std::unique_ptr<DeferredLightSplitScreenShaderGroup> m_d_lightSplitScreenShaders;
	std::unique_ptr<DeferredLightMultipleLightsShaderGroup> m_d_lightMultipleLightsShaders;
	std::unique_ptr<DeferredLightMultipleShadowLightsShaderGroup> m_d_lightMultipleShadowLightsShaders;
	std::unique_ptr<DeferredTextureChunkShaderGroup> m_d_textureChunkShaders;

	ShaderType m_currentShaderType;
};

#endif