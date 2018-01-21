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
#include "DeferredLightShaderGroup.hpp"

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
	{
		return false;
	}

	m_d_colorShaders = std::make_unique<DeferredSingleColorShaderGroup>();
	if (!m_d_colorShaders->Initialize(device))
	{
		return false;
	}

	m_d_lightShaders = std::make_unique<DeferredLightShaderGroup>();
	if (!m_d_lightShaders->Initialize(device))
	{
		return false;
	}

	m_depthShaders = std::make_unique<DepthShaderGroup>();
	if (!m_depthShaders->Initialize(device))
	{
		return false;
	}

	m_s_shaders = std::make_unique<DeferredShadowShaderGroup>();
	if (!m_s_shaders->Initialize(device))
	{
		return false;
	}

	m_textureShaders = std::make_unique<TextureShaderGroup>();
	if (!m_textureShaders->Initialize(device))
	{
		return false;
	}

	m_d_textureShaders = std::make_unique<DeferredTextureShaderGroup>();
	if (!m_d_textureShaders->Initialize(device))
	{
		return false;
	}

	m_HUDShaders = std::make_unique<HUDShaderGroup>();
	if (!m_HUDShaders->Initialize(device))
	{
		return false;
	}

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
		m_d_lightShaders->SetupShaders(deviceContext);
		break;
	case ShaderType::DEPTH:
		m_depthShaders->SetupShaders(deviceContext);
		break;
	case ShaderType::D_SHADOW:
		m_s_shaders->SetupShaders(deviceContext);
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

void ShaderManager::SetPerFrameSingleColorConstantBuffer(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & viewMatrix, const DirectX::XMMATRIX & projectionMatrix, const DirectX::XMMATRIX & lightViewMatrix, const DirectX::XMMATRIX & lightProjectionMatrix, Vector3f lightPosition, float lightIntensity)
{
	m_colorShaders->SetupPerFrameBuffer(deviceContext, viewMatrix, projectionMatrix, lightPosition, lightViewMatrix, lightProjectionMatrix, lightIntensity);
}
void ShaderManager::SetPerFrameTextureConstantBuffer(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & viewMatrix, const DirectX::XMMATRIX & projectionMatrix, Vector3f lightPosition, float lightIntensity)
{
	m_textureShaders->SetupPerFrameBuffer(deviceContext, viewMatrix, projectionMatrix, lightPosition, lightIntensity);
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
	m_s_shaders->SetupPerFrameBuffer(deviceContext, lightViewMatrix, lightProjectionMatrix);
}
void ShaderManager::SetPerFrameDeferredLightConstantBuffer(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & lightViewMatrix, const DirectX::XMMATRIX & lightProjectionMatrix, ID3D11ShaderResourceView* depthTexture, Vector3f lightPosition, float lightIntensity)
{
	m_d_lightShaders->SetupPerFrameBuffer(deviceContext, BufferType::NR_OF_D_ELEMENTS, Direct3D::Get()->GetDeferredShaderResourceViews(), depthTexture, lightPosition, lightViewMatrix, lightProjectionMatrix, lightIntensity);
}

void ShaderManager::SetPerObjectSingleColorConstantBuffer(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & worldMatrix, Vector3f color)
{
	m_colorShaders->SetupPerObjectBuffer(deviceContext, worldMatrix, color);
}
void ShaderManager::SetPerObjectTextureConstantBuffer(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX& worldMatrix, ID3D11ShaderResourceView* bitMapTexture)
{
	m_textureShaders->SetupPerObjectBuffer(deviceContext, worldMatrix, bitMapTexture);
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
void ShaderManager::SetPerObjectDeferredTextureConstantBuffer(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & worldMatrix, ID3D11ShaderResourceView * texture)
{
	m_d_textureShaders->SetupPerObjectBuffer(deviceContext, worldMatrix, texture);
}
void ShaderManager::SetPerObjectDeferredShadowConstantBuffer(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX & worldMatrix)
{
	m_s_shaders->SetupPerObjectBuffer(deviceContext, worldMatrix);
}