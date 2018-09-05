#include "BaseShader.hpp"

BaseShader::BaseShader() :
	m_device(nullptr), m_deviceContext(nullptr),
	m_vertexShader(nullptr), m_geometryShader(nullptr), m_pixelShader(nullptr),
	m_inputLayout(nullptr)
{
}

BaseShader::~BaseShader()
{
}

bool BaseShader::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext)
{
	m_device = device;
	m_deviceContext = deviceContext;

	return true;
}
