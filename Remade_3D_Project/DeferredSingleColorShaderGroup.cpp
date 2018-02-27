#include "DeferredSingleColorShaderGroup.hpp"
#include <d3d11.h>

DeferredSingleColorShaderGroup::DeferredSingleColorShaderGroup()
{
}

DeferredSingleColorShaderGroup::~DeferredSingleColorShaderGroup()
{
}

bool DeferredSingleColorShaderGroup::Initialize(ID3D11Device * device)
{
	m_vertexShaderName = "VS_PosNormColor.hlsl";
	m_geometryShaderName = "NULL";
	m_pixelShaderName = "PS_D_SingleColor.hlsl";

	if (!ShaderGroup::Initialize(device))
		return false;

	return true;
}

void DeferredSingleColorShaderGroup::SetupShaders(ID3D11DeviceContext * deviceContext)
{
	ShaderGroup::SetupShaders(deviceContext);

	// Might need to set sample state here
}

void DeferredSingleColorShaderGroup::SetupPerFrameBuffer(ID3D11DeviceContext * deviceContext)
{
}

void DeferredSingleColorShaderGroup::SetupPerObjectBuffer(ID3D11DeviceContext * deviceContext)
{
}