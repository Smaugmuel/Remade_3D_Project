#include "ShaderManager.hpp"

#include <d3d11.h>

#include "Direct3D.hpp"

#include "SingleColorShaderGroup.hpp"
#include "TextureShaderGroup.hpp"
#include "DepthShaderGroup.hpp"
#include "HUDShaderGroup.hpp"
#include "DeferredSingleColorShaderGroup.hpp"
#include "DeferredTextureShaderGroup.hpp"
#include "DeferredShadowShaderGroup.hpp"
#include "DeferredLightShadowShaderGroup.hpp"
#include "DeferredLightSplitScreenShaderGroup.hpp"
#include "DeferredLightMultipleLightsShaderGroup.hpp"
#include "DeferredLightMultipleShadowLightsShaderGroup.hpp"
#include "DeferredTextureChunkShaderGroup.hpp"

ShaderManager* Singleton<ShaderManager>::s_instance = nullptr;

ShaderManager::ShaderManager()
{
}
ShaderManager::~ShaderManager()
{
}

bool ShaderManager::Initialize(ID3D11Device* device)
{
	m_colorShaders = std::make_unique<SingleColorShaderGroup>();
	if (!m_colorShaders->Initialize(device))
		return false;

	m_textureShaders = std::make_unique<TextureShaderGroup>();
	if (!m_textureShaders->Initialize(device))
		return false;

	m_depthShaders = std::make_unique<DepthShaderGroup>();
	if (!m_depthShaders->Initialize(device))
		return false;

	m_HUDShaders = std::make_unique<HUDShaderGroup>();
	if (!m_HUDShaders->Initialize(device))
		return false;

	m_d_colorShaders = std::make_unique<DeferredSingleColorShaderGroup>();
	if (!m_d_colorShaders->Initialize(device))
		return false;

	m_d_textureShaders = std::make_unique<DeferredTextureShaderGroup>();
	if (!m_d_textureShaders->Initialize(device))
		return false;

	m_d_s_shaders = std::make_unique<DeferredShadowShaderGroup>();
	if (!m_d_s_shaders->Initialize(device))
		return false;

	m_d_lightShadowShaders = std::make_unique<DeferredLightShadowShaderGroup>();
	if (!m_d_lightShadowShaders->Initialize(device))
		return false;

	m_d_lightSplitScreenShaders = std::make_unique<DeferredLightSplitScreenShaderGroup>();
	if (!m_d_lightSplitScreenShaders->Initialize(device))
		return false;

	m_d_lightMultipleLightsShaders = std::make_unique<DeferredLightMultipleLightsShaderGroup>();
	if (!m_d_lightMultipleLightsShaders->Initialize(device))
		return false;

	m_d_lightMultipleShadowLightsShaders = std::make_unique<DeferredLightMultipleShadowLightsShaderGroup>();
	if (!m_d_lightMultipleShadowLightsShaders->Initialize(device))
		return false;

	m_d_textureChunkShaders = std::make_unique<DeferredTextureChunkShaderGroup>();
	if (!m_d_textureChunkShaders->Initialize(device))
		return false;

	m_currentShaderType = (ShaderType)-1;

	return true;
}

void ShaderManager::SetShaderType(ID3D11DeviceContext* deviceContext, const ShaderType& shaderType)
{
	if (shaderType == m_currentShaderType)
	{
		return;
	}

	switch (shaderType)
	{
	case ShaderType::SINGLE_COLOR:
		m_colorShaders.get()->SetupShaders(deviceContext);
		break;
	case ShaderType::D_SINGLE_COLOR:
		m_d_colorShaders.get()->SetupShaders(deviceContext);
		break;
	case ShaderType::D_LIGHT:
		m_d_lightShadowShaders.get()->SetupShaders(deviceContext);
		break;
	case ShaderType::DEPTH:
		m_depthShaders.get()->SetupShaders(deviceContext);
		break;
	case ShaderType::D_SHADOW:
		m_d_s_shaders.get()->SetupShaders(deviceContext);
		break;
	case ShaderType::TEXTURE:
		m_textureShaders.get()->SetupShaders(deviceContext);
		break;
	case ShaderType::D_TEXTURE:
		m_d_textureShaders.get()->SetupShaders(deviceContext);
		break;
	case ShaderType::HUD:
		m_HUDShaders.get()->SetupShaders(deviceContext);
		break;
	case ShaderType::D_SPLIT:
		m_d_lightSplitScreenShaders.get()->SetupShaders(deviceContext);
		break;
	case ShaderType::D_MULTIPLE:
		m_d_lightMultipleLightsShaders.get()->SetupShaders(deviceContext);
		break;
	case ShaderType::D_MULTIPLE_SHADOWS:
		m_d_lightMultipleShadowLightsShaders.get()->SetupShaders(deviceContext);
		break;
	case ShaderType::D_TEXTURE_CHUNK:
		m_d_textureChunkShaders.get()->SetupShaders(deviceContext);
		break;
	default:
		break;
	}

	m_currentShaderType = shaderType;


	//if (m_shaderGroupMap.find(shaderGroupName) != m_shaderGroupMap.end())
	//{
	//	deviceContext->VSSetShader(m_shaderGroupMap[shaderGroupName]->vertexShader->GetShader(), nullptr, 0);
	//	deviceContext->HSSetShader(nullptr, nullptr, 0);
	//	deviceContext->DSSetShader(nullptr, nullptr, 0);
	//	deviceContext->GSSetShader(nullptr, nullptr, 0);
	//	//deviceContext->GSSetShader(m_shaderGroupMap[shaderGroupName]->vgeometryShader->GetShader(), nullptr, 0);
	//	deviceContext->PSSetShader(m_shaderGroupMap[shaderGroupName]->pixelShader->GetShader(), nullptr, 0);
	//
	//	deviceContext->IASetInputLayout(m_shaderGroupMap[shaderGroupName]->vertexShader->GetInputLayout());
	//
	//	m_currentShaderGroup = shaderGroupName;
	//
	//	return true;
	//}
}

void ShaderManager::SetPerFrameSingleColorConstantBuffer(ID3D11DeviceContext * deviceContext, Vector3f lightPosition, float lightIntensity)
{
	m_colorShaders.get()->SetupPerFrameBuffer(deviceContext);
}
void ShaderManager::SetPerFrameTextureConstantBuffer(ID3D11DeviceContext * deviceContext, Vector3f lightPosition, float lightIntensity)
{
	m_textureShaders.get()->SetupPerFrameBuffer(deviceContext);
}
void ShaderManager::SetPerFrameDepthConstantBuffer(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & viewMatrix, const DirectX::XMMATRIX & projectionMatrix)
{
	m_depthShaders.get()->SetupPerFrameBuffer(deviceContext, viewMatrix, projectionMatrix);
}
void ShaderManager::SetPerFrameDeferredSingleColorConstantBuffer(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & viewMatrix, const DirectX::XMMATRIX & projectionMatrix)
{
	m_d_colorShaders.get()->SetupPerFrameBuffer(deviceContext);
}
void ShaderManager::SetPerFrameDeferredTextureConstantBuffer(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & viewMatrix, const DirectX::XMMATRIX & projectionMatrix)
{
	m_d_textureShaders.get()->SetupPerFrameBuffer(deviceContext);
}
void ShaderManager::SetPerFrameDeferredShadowConstantBuffer(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & lightViewMatrix, const DirectX::XMMATRIX & lightProjectionMatrix)
{
	m_d_s_shaders.get()->SetupPerFrameBuffer(deviceContext, lightViewMatrix, lightProjectionMatrix);
}
void ShaderManager::SetPerFrameDeferredLightShadowConstantBuffer(ID3D11DeviceContext * deviceContext, unsigned int nrOfDeferredBuffers, ID3D11ShaderResourceView** deferredShaderResourceViews, ID3D11ShaderResourceView* depthTexture)
{
	m_d_lightShadowShaders.get()->SetupPerFrameBuffer(deviceContext, nrOfDeferredBuffers, deferredShaderResourceViews, depthTexture);
}
void ShaderManager::SetPerFrameDeferredLightSplitScreenConstantBuffer(ID3D11DeviceContext * deviceContext, unsigned int nrOfDeferredBuffers, ID3D11ShaderResourceView** deferredShaderResourceViews, ID3D11ShaderResourceView * depthTexture)
{
	m_d_lightSplitScreenShaders.get()->SetupPerFrameBuffer(deviceContext, nrOfDeferredBuffers, deferredShaderResourceViews, depthTexture/*, lightPosition, lightViewMatrix, lightProjectionMatrix, lightIntensity*/);
}
void ShaderManager::SetPerFrameDeferredLightMultipleLightsConstantBuffer(ID3D11DeviceContext * deviceContext, unsigned int nrOfDeferredBuffers, ID3D11ShaderResourceView ** deferredShaderResourceViews)
{
	m_d_lightMultipleLightsShaders.get()->SetupPerFrameBuffer(deviceContext, nrOfDeferredBuffers, deferredShaderResourceViews);
}
void ShaderManager::SetPerFrameDeferredLightMultipleShadowLightsConstantBuffer(ID3D11DeviceContext * deviceContext, unsigned int nrOfResources, ID3D11ShaderResourceView ** resources, ID3D11ShaderResourceView ** depthTextures)
{
	m_d_lightMultipleShadowLightsShaders.get()->SetupPerFrameBuffer(deviceContext, nrOfResources, resources, depthTextures);
}

void ShaderManager::SetPerObjectSingleColorConstantBuffer(ID3D11DeviceContext * deviceContext)
{
	m_colorShaders.get()->SetupPerObjectBuffer(deviceContext);
}
void ShaderManager::SetPerObjectTextureConstantBuffer(ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView* bitMapTexture)
{
	m_textureShaders.get()->SetupPerObjectBuffer(deviceContext, bitMapTexture);
}
void ShaderManager::SetPerObjectDepthConstantBuffer(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & worldMatrix)
{
	m_depthShaders.get()->SetupPerObjectBuffer(deviceContext, worldMatrix);
}
void ShaderManager::SetPerObjectHUDConstantBuffer(ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture)
{
	m_HUDShaders.get()->SetupPerObjectBuffer(deviceContext, texture);
}
void ShaderManager::SetPerObjectDeferredSingleColorConstantBuffer(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & worldMatrix, Vector3f color)
{
	m_d_colorShaders.get()->SetupPerObjectBuffer(deviceContext);
}
void ShaderManager::SetPerObjectDeferredTextureConstantBuffer(ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture)
{
	m_d_textureShaders.get()->SetupPerObjectBuffer(deviceContext, texture);
}
void ShaderManager::SetPerObjectDeferredShadowConstantBuffer(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & worldMatrix)
{
	m_d_s_shaders.get()->SetupPerObjectBuffer(deviceContext, worldMatrix);
}

void ShaderManager::SetPerObjectDeferredTextureChunkShaderGroup(ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture)
{
	m_d_textureChunkShaders.get()->SetupPerObjectBuffer(deviceContext, texture);
}