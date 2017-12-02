#include "ShaderManager.hpp"

#include <d3d11.h>

#include "Camera.hpp"
#include "Object.hpp"
#include "Direct3D.hpp"
#include "SingleColorShaderGroup.hpp"
#include "DeferredSingleColorShaderGroup.hpp"
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

void ShaderManager::SetPerFrameConstantBuffer(ID3D11DeviceContext* deviceContext, Camera* camera, Vector3f lightPos, float lightIntensity)
{
	switch (m_currentShaderType)
	{
	case ShaderType::SINGLE_COLOR:
		m_colorShaders->SetupPerFrameBuffer(deviceContext, camera);
		break;
	case ShaderType::D_SINGLE_COLOR:
		m_d_colorShaders->SetupPerFrameBuffer(deviceContext, camera);
		break;
	case ShaderType::D_LIGHT:
		m_d_lightShaders->SetupPerFrameBuffer(deviceContext, BufferType::NR_OF_ELEMENTS, Direct3D::Get()->GetShaderResourceViews(), lightPos, lightIntensity);
		break;
	default:
		break;
	}

	//m_shaderGroupMap[m_currentShaderGroup]->vertexShader->SetPerFrameConstantBuffer(deviceContext, camera);
}

void ShaderManager::SetPerObjectConstantBuffer(ID3D11DeviceContext* deviceContext, Object* object)
{
	switch (m_currentShaderType)
	{
	case ShaderType::SINGLE_COLOR:
		m_colorShaders->SetupPerObjectBuffer(deviceContext, object);
		break;
	case ShaderType::D_SINGLE_COLOR:
		m_d_colorShaders->SetupPerObjectBuffer(deviceContext, object);
		break;
	default:
		break;
	}

	//m_shaderGroupMap[m_currentShaderGroup]->vertexShader->SetPerObjectConstantBuffer(deviceContext, object);
}