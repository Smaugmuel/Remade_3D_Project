#include "LineShaderGroup.hpp"

LineShaderGroup::LineShaderGroup()
{
}

LineShaderGroup::~LineShaderGroup()
{
}

bool LineShaderGroup::Initialize(ID3D11Device * device)
{
	m_vertexShaderName = "VS_PosColor.hlsl";
	m_geometryShaderName = "NULL";
	m_pixelShaderName = "PS_Line.hlsl";
	
	if (!ShaderGroup::Initialize(device))
		return false;

	return true;
}

void LineShaderGroup::SetupShaders(ID3D11DeviceContext * deviceContext)
{
	ShaderGroup::SetupShaders(deviceContext);
}

void LineShaderGroup::SetupPerFrameBuffer(ID3D11DeviceContext * deviceContext)
{
}

void LineShaderGroup::SetupPerObjectBuffer(ID3D11DeviceContext * deviceContext)
{
}