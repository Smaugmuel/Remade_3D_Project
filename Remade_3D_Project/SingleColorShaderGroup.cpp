#include "SingleColorShaderGroup.hpp"
#include <d3d11.h>
#include "ConstantBufferTypes.hpp"

SingleColorShaderGroup::SingleColorShaderGroup()
{
}

SingleColorShaderGroup::~SingleColorShaderGroup()
{
	if (m_psPerFrameBuffer)
	{
		m_psPerFrameBuffer->Release();
		m_psPerFrameBuffer = nullptr;
	}
}

bool SingleColorShaderGroup::Initialize(ID3D11Device* device)
{
	D3D11_BUFFER_DESC ps_perFrameDesc;

	m_vertexShaderName = "VS_PosNormColor.hlsl";
	m_pixelShaderName = "PS_SingleColor.hlsl";

	if (!ShaderGroup::Initialize(device))
		return false;

	// Create per-frame pixel shader constant buffer ==========================================================
	memset(&ps_perFrameDesc, 0, sizeof(ps_perFrameDesc));
	ps_perFrameDesc.Usage = D3D11_USAGE_DYNAMIC;
	ps_perFrameDesc.ByteWidth = sizeof(PS_PerFrameBuffer);
	ps_perFrameDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ps_perFrameDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ps_perFrameDesc.MiscFlags = 0;
	ps_perFrameDesc.StructureByteStride = 0;

	if (FAILED(device->CreateBuffer(&ps_perFrameDesc, nullptr, &m_psPerFrameBuffer)))
	{
		return false;
	}

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