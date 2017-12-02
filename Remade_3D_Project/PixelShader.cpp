#include "PixelShader.hpp"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>

PixelShader::PixelShader()
{
}

PixelShader::~PixelShader()
{
}

bool PixelShader::Initialize(ID3D11Device* device, const char* shaderType)
{
	ID3D10Blob* pixelShaderBuffer;
	HRESULT result;

	//m_device = device;

	std::string str((std::string)"PS_" + shaderType + ".hlsl");	// Add prefix and suffix
	std::wstring wstr(str.size(), L' ');						// Set size (has to be done) and add the L
	std::copy(str.begin(), str.end(), wstr.begin());			// Copy to wstring

	result = D3DCompileFromFile(
		wstr.c_str(),
		nullptr,
		nullptr,
		"main",
		"ps_5_0",
		0,
		0,
		&pixelShaderBuffer,
		nullptr
		);
	if (FAILED(result))
	{
		return false;
	}

	result = device->CreatePixelShader(
		pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(),
		nullptr,
		&m_pixelShader
		);
	if (FAILED(result))
	{
		return false;
	}

	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;

	return true;
}

ID3D11PixelShader* PixelShader::GetShader() const
{
	return m_pixelShader;
}
