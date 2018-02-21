#include "SingleColorShaderGroup.hpp"
#include <d3d11.h>

SingleColorShaderGroup::SingleColorShaderGroup()
{
}

SingleColorShaderGroup::~SingleColorShaderGroup()
{
}

bool SingleColorShaderGroup::Initialize(ID3D11Device* device)
{
	m_vertexShaderName = "VS_PosNormColor.hlsl";
	m_pixelShaderName = "PS_SingleColor.hlsl";

	if (!ShaderGroup::Initialize(device))
		return false;

	return true;
}

void SingleColorShaderGroup::SetupShaders(ID3D11DeviceContext* deviceContext)
{
	ShaderGroup::SetupShaders(deviceContext);
}
void SingleColorShaderGroup::SetupPerFrameBuffer(ID3D11DeviceContext * deviceContext)
{
}
void SingleColorShaderGroup::SetupPerObjectBuffer(ID3D11DeviceContext* deviceContext)
{
}