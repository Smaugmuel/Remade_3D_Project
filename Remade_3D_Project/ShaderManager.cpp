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
		m_colorShaders->SetupShaders(deviceContext);
		break;
	case ShaderType::D_SINGLE_COLOR:
		m_d_colorShaders->SetupShaders(deviceContext);
		break;
	case ShaderType::D_LIGHT:
		m_d_lightShadowShaders->SetupShaders(deviceContext);
		break;
	case ShaderType::DEPTH:
		m_depthShaders->SetupShaders(deviceContext);
		break;
	case ShaderType::D_SHADOW:
		m_d_s_shaders->SetupShaders(deviceContext);
		break;
	case ShaderType::TEXTURE:
		m_textureShaders->SetupShaders(deviceContext);
		break;
	case ShaderType::D_TEXTURE:
		m_d_textureShaders->SetupShaders(deviceContext);
		break;
	case ShaderType::HUD:
		m_HUDShaders->SetupShaders(deviceContext);
		break;
	case ShaderType::D_SPLIT:
		m_d_lightSplitScreenShaders->SetupShaders(deviceContext);
		break;
	case ShaderType::D_MULTIPLE:
		m_d_lightMultipleLightsShaders->SetupShaders(deviceContext);
		break;
	case ShaderType::D_MULTIPLE_SHADOWS:
		m_d_lightMultipleShadowLightsShaders->SetupShaders(deviceContext);
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

	//	deviceContext->IASetInputLayout(m_shaderGroupMap[shaderGroupName]->vertexShader->GetInputLayout());

	//	m_currentShaderGroup = shaderGroupName;

	//	return true;
	//}
}

void ShaderManager::SetPerFrameSingleColorConstantBuffer(ID3D11DeviceContext * deviceContext,/* const DirectX::XMMATRIX & viewMatrix, const DirectX::XMMATRIX & projectionMatrix,*/ Vector3f lightPosition, float lightIntensity)
{
	m_colorShaders->SetupPerFrameBuffer(deviceContext/*, viewMatrix, projectionMatrix, lightPosition, lightIntensity*/);
}
void ShaderManager::SetPerFrameTextureConstantBuffer(ID3D11DeviceContext * deviceContext, Vector3f lightPosition, float lightIntensity)
{
	m_textureShaders->SetupPerFrameBuffer(deviceContext, lightPosition, lightIntensity);
}
void ShaderManager::SetPerFrameDepthConstantBuffer(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & viewMatrix, const DirectX::XMMATRIX & projectionMatrix)
{
	m_depthShaders->SetupPerFrameBuffer(deviceContext, viewMatrix, projectionMatrix);
}
void ShaderManager::SetPerFrameDeferredSingleColorConstantBuffer(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & viewMatrix, const DirectX::XMMATRIX & projectionMatrix)
{
	m_d_colorShaders->SetupPerFrameBuffer(deviceContext, viewMatrix, projectionMatrix);
}
void ShaderManager::SetPerFrameDeferredTextureConstantBuffer(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & viewMatrix, const DirectX::XMMATRIX & projectionMatrix)
{
	m_d_textureShaders->SetupPerFrameBuffer(deviceContext, viewMatrix, projectionMatrix);
}
void ShaderManager::SetPerFrameDeferredShadowConstantBuffer(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & lightViewMatrix, const DirectX::XMMATRIX & lightProjectionMatrix)
{
	m_d_s_shaders->SetupPerFrameBuffer(deviceContext, lightViewMatrix, lightProjectionMatrix);
}
void ShaderManager::SetPerFrameDeferredLightShadowConstantBuffer(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & lightViewMatrix, const DirectX::XMMATRIX & lightProjectionMatrix, unsigned int nrOfDeferredBuffers, ID3D11ShaderResourceView** deferredShaderResourceViews, ID3D11ShaderResourceView* depthTexture, Vector3f lightPosition, float lightIntensity)
{
	m_d_lightShadowShaders->SetupPerFrameBuffer(deviceContext, nrOfDeferredBuffers, deferredShaderResourceViews, depthTexture, lightPosition, lightViewMatrix, lightProjectionMatrix, lightIntensity);
}
void ShaderManager::SetPerFrameDeferredLightSplitScreenConstantBuffer(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & lightViewMatrix, const DirectX::XMMATRIX & lightProjectionMatrix, unsigned int nrOfDeferredBuffers, ID3D11ShaderResourceView** deferredShaderResourceViews, ID3D11ShaderResourceView * depthTexture, Vector3f lightPosition, float lightIntensity)
{
	m_d_lightSplitScreenShaders->SetupPerFrameBuffer(deviceContext, nrOfDeferredBuffers, deferredShaderResourceViews, depthTexture, lightPosition, lightViewMatrix, lightProjectionMatrix, lightIntensity);
}
void ShaderManager::SetPerFrameDeferredLightMultipleLightsConstantBuffer(ID3D11DeviceContext * deviceContext, unsigned int nrOfDeferredBuffers, ID3D11ShaderResourceView ** deferredShaderResourceViews, Vector3f lightPositions[MAX_NR_OF_LIGHTS], float lightIntensitys[MAX_NR_OF_LIGHTS])
{
	m_d_lightMultipleLightsShaders->SetupPerFrameBuffer(deviceContext, nrOfDeferredBuffers, deferredShaderResourceViews, lightPositions, lightIntensitys);
}

void ShaderManager::SetPerFrameDeferredLightMultipleShadowLightsConstantBuffer(ID3D11DeviceContext * deviceContext, unsigned int nrOfResources, ID3D11ShaderResourceView ** resources, unsigned int nrOfLights, ID3D11ShaderResourceView * depthTextures[MAX_NR_OF_LIGHTS], Vector3f lightPositions[MAX_NR_OF_LIGHTS], DirectX::XMMATRIX lightViewMatrices[MAX_NR_OF_LIGHTS], DirectX::XMMATRIX lightProjectionMatrices[MAX_NR_OF_LIGHTS], float lightIntensities[MAX_NR_OF_LIGHTS])
{
	m_d_lightMultipleShadowLightsShaders->SetupPerFrameBuffer(deviceContext, nrOfResources, resources, nrOfLights, depthTextures, lightPositions, lightViewMatrices, lightProjectionMatrices, lightIntensities);
}

void ShaderManager::SetPerObjectSingleColorConstantBuffer(ID3D11DeviceContext * deviceContext, /*const DirectX::XMMATRIX & worldMatrix,*/ Vector3f color)
{
	m_colorShaders->SetupPerObjectBuffer(deviceContext/*, worldMatrix, color*/);
}
void ShaderManager::SetPerObjectTextureConstantBuffer(ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView* bitMapTexture)
{
	m_textureShaders->SetupPerObjectBuffer(deviceContext, bitMapTexture);
}
void ShaderManager::SetPerObjectDepthConstantBuffer(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & worldMatrix)
{
	m_depthShaders->SetupPerObjectBuffer(deviceContext, worldMatrix);
}
void ShaderManager::SetPerObjectHUDConstantBuffer(ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture)
{
	m_HUDShaders->SetupPerObjectBuffer(deviceContext, texture);
}
void ShaderManager::SetPerObjectDeferredSingleColorConstantBuffer(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & worldMatrix, Vector3f color)
{
	m_d_colorShaders->SetupPerObjectBuffer(deviceContext, worldMatrix, color);
}
void ShaderManager::SetPerObjectDeferredTextureConstantBuffer(ID3D11DeviceContext * deviceContext, /*const DirectX::XMMATRIX & worldMatrix,*/ ID3D11ShaderResourceView * texture)
{
	m_d_textureShaders->SetupPerObjectBuffer(deviceContext, /*worldMatrix,*/ texture);
}
void ShaderManager::SetPerObjectDeferredShadowConstantBuffer(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & worldMatrix)
{
	m_d_s_shaders->SetupPerObjectBuffer(deviceContext, worldMatrix);
}