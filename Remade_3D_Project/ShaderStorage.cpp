#include "ShaderStorage.hpp"
#include <d3d11.h>
#include <d3dcompiler.h>

ShaderStorage* Singleton<ShaderStorage>::s_instance = nullptr;

ShaderStorage::ShaderStorage()
{
}

ShaderStorage::~ShaderStorage()
{
}

bool ShaderStorage::Initialize(ID3D11Device* device)
{
	return false;
}

bool ShaderStorage::LoadShader(wchar_t * shaderName)
{
	return false;
}

//bool ShaderStorage::CreateVertexShader(ID3D11Device* device, wchar_t* shaderName)
//{
//	ID3D10Blob* vertexShaderBuffer;
//	HRESULT result;
//
//	result = D3DCompileFromFile(
//		shaderName,
//		nullptr,
//		nullptr,
//		"main",
//		"vs_5_0",
//		0,
//		0,
//		&vertexShaderBuffer,
//		nullptr
//		);
//	if (FAILED(result))
//	{
//		return false;
//	}
//
//	//result = device->CreateVertexShader(
//	//	vertexShaderBuffer->GetBufferPointer(),
//	//	vertexShaderBuffer->GetBufferSize(),
//	//	nullptr,
//	//	&m_vertexShader
//	//	);
//	//if (FAILED(result))
//	//{
//	//	return false;
//	//}
//
//	//if (!InitializeInputLayout(vertexShaderBuffer))
//	//{
//	//	return false;
//	//}
//
//	vertexShaderBuffer->Release();
//	vertexShaderBuffer = nullptr;
//
//	return true;
//
//	return false;
//}
//
//bool ShaderStorage::CreateGeometryShader(ID3D11Device* device, wchar_t* shaderName)
//{
//	return false;
//}
//
//bool ShaderStorage::CreatePixelShader(ID3D11Device* device, wchar_t* shaderName)
//{
//	return false;
//}
//
//Shader* ShaderStorage::GetShader(wchar_t* shaderName) const
//{
//	return nullptr;
//}
