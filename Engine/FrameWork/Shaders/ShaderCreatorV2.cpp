#include "ShaderCreatorV2.hpp"
#include <d3d11.h>
#include <d3dcompiler.h>
#include "../../Misc/StringConverter.hpp"

ShaderCreatorV2::ShaderCreatorV2()
{
}

ShaderCreatorV2::~ShaderCreatorV2()
{
}

bool ShaderCreatorV2::Initialize(ID3D11Device * device)
{
	m_device = device;
	return true;
}

VertexShaderData ShaderCreatorV2::CompileAndCreateVertexShader(int nrOfDefines, ShaderDefine* defines)
{
	D3D_SHADER_MACRO* macros = nullptr;
	HRESULT result;
	VertexShaderData shaderData;

	if (nrOfDefines > 0)
	{
		/*
		Allocate one extra macro to null-terminate it
		*/
		macros = new D3D_SHADER_MACRO[nrOfDefines + 1];

		/*
		Copy data
		*/
		for (int i = 0; i < nrOfDefines; i++)
		{
			macros[i].Name = defines[i].name;
			macros[i].Definition = defines[i].value;
		}

		/*
		The last macro must be null-terminated
		*/
		macros[nrOfDefines].Name = nullptr;
		macros[nrOfDefines].Definition = nullptr;
	}

	/*
	Compile shader
	*/
	result = D3DCompileFromFile(
		L"../Engine/FrameWork/HLSL/VS_Generic.hlsl",
		macros,
		nullptr,
		"main",
		"vs_5_0",
		0,
		0,
		&shaderData.blob,
		nullptr
	);
	if (FAILED(result))
	{
		if (macros)
			delete[] macros;
		return shaderData;
	}

	/*
	Create shader
	*/
	result = m_device->CreateVertexShader(
		shaderData.blob->GetBufferPointer(),
		shaderData.blob->GetBufferSize(),
		nullptr,
		&shaderData.vs
	);
	if (FAILED(result))
	{
		if (macros)
			delete[] macros;
		return shaderData;
	}

	if (macros)
		delete[] macros;
	return shaderData;
}

ID3D11PixelShader * ShaderCreatorV2::CompileAndCreatePixelShader(int nrOfDefines, ShaderDefine* defines)
{
	return nullptr;
}

ID3D11PixelShader * ShaderCreatorV2::CompileAndCreatePixelShaderFromFile(const char* fileName, int nrOfDefines, ShaderDefine * defines)
{
	HRESULT result;
	D3D_SHADER_MACRO* macros = nullptr;
	ID3D10Blob* blob = nullptr;
	ID3D11PixelShader* shader = nullptr;

	std::optional<std::wstring> wName = StringConverter::ToWideString(fileName);
	if (!wName)
		return nullptr;

	std::wstring fullName = L"../Engine/FrameWork/HLSL/" + wName.value();

	if (nrOfDefines > 0)
	{
		/*
		Allocate one extra macro to null-terminate it
		*/
		macros = new D3D_SHADER_MACRO[nrOfDefines + 1];

		/*
		Copy data
		*/
		for (int i = 0; i < nrOfDefines; i++)
		{
			macros[i].Name = defines[i].name;
			macros[i].Definition = defines[i].value;
		}

		/*
		The last macro must be null-terminated
		*/
		macros[nrOfDefines].Name = nullptr;
		macros[nrOfDefines].Definition = nullptr;
	}

	/*
	Compile shader
	*/
	result = D3DCompileFromFile(
		fullName.c_str(),
		macros,
		nullptr,
		"main",
		"ps_5_0",
		0,
		0,
		&blob,
		nullptr
	);
	if (FAILED(result))
	{
		if (macros)
			delete[] macros;
		return nullptr;
	}

	/*
	Create shader
	*/
	result = m_device->CreatePixelShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		nullptr,
		&shader
	);
	if (FAILED(result))
	{
		if (macros)
			delete[] macros;
		return nullptr;
	}
	blob->Release();

	if (macros)
		delete[] macros;
	return shader;
}

//int ShaderCompiler::CreateShader(ShaderType type, unsigned int flags)
//{
//	std::wstring path = L"../Engine/FrameWork/HLSL/";
//	std::string target = "";
//	std::vector<std::string> macrosVector;
//	D3D_SHADER_MACRO* macros = nullptr;
//
//
//	switch (type)
//	{
//	case ShaderType::VS:
//		path += L"VS_GeometryGeneric.hlsl";
//		target = "vs_5_0";
//		break;
//	case ShaderType::HS:
//		path += L"HS_GeometryGeneric.hlsl";
//		target = "hs_5_0";
//		break;
//	case ShaderType::DS:
//		path += L"DS_GeometryGeneric.hlsl";
//		target = "ds_5_0";
//		break;
//	case ShaderType::GS:
//		path += L"GS_GeometryGeneric.hlsl";
//		target = "gs_5_0";
//		break;
//	case ShaderType::PS:
//		path += L"PS_GeometryGeneric.hlsl";
//		target = "ps_5_0";
//		break;
//	case ShaderType::CS:
//		path += L"CS_GeometryGeneric.hlsl";
//		target = "cs_5_0";
//		break;
//	}
//
//	
//
//	return 0;
//}