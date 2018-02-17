#include "ShaderGroup.hpp"
#include <d3d11.h>
#include "ShaderStorage.hpp"

ShaderGroup::ShaderGroup()
{
}

ShaderGroup::~ShaderGroup()
{
}

bool ShaderGroup::Initialize(ID3D11Device * device)
{
	if (!ShaderStorage::Get()->CreateVertexShader(device, m_vertexShaderName))
		return false;
	if (!ShaderStorage::Get()->CreatePixelShader(device, m_pixelShaderName))
		return false;

	return true;
}

void ShaderGroup::SetupShaders(ID3D11DeviceContext * deviceContext)
{
	ShaderStorage* storage = ShaderStorage::Get();

	deviceContext->VSSetShader(storage->GetVertexShader(m_vertexShaderName), nullptr, 0);
	deviceContext->HSSetShader(nullptr, nullptr, 0);
	deviceContext->DSSetShader(nullptr, nullptr, 0);
	deviceContext->GSSetShader(nullptr, nullptr, 0);
	deviceContext->PSSetShader(storage->GetPixelShader(m_pixelShaderName), nullptr, 0);

	deviceContext->IASetInputLayout(storage->GetInputLayout(m_vertexShaderName));
}