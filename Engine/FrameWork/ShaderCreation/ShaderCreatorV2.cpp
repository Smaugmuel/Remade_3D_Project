#include "ShaderCreatorV2.hpp"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <memory>
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
	const wchar_t* wPath = L"../Engine/FrameWork/HLSL/VS_Generic.hlsl";
	std::unique_ptr<_D3D_SHADER_MACRO[]> macros = nullptr;
	_D3D_SHADER_MACRO* macrosRaw = nullptr;
	VertexShaderData shaderData;

	if (nrOfDefines > 0)
	{
		/*
		Allocate one extra macro to null-terminate it
		*/
		macros = std::make_unique<_D3D_SHADER_MACRO[]>(nrOfDefines + 1U);
		macrosRaw = macros.get();

		/*
		Copy data
		*/
		for (int i = 0; i < nrOfDefines; i++)
		{
			macrosRaw[i].Name = defines[i].name;
			macrosRaw[i].Definition = defines[i].value;
		}

		/*
		The last macro must be null-terminated
		*/
		macrosRaw[nrOfDefines].Name = nullptr;
		macrosRaw[nrOfDefines].Definition = nullptr;
	}

	/*
	Compile shader
	*/
	if (FAILED(D3DCompileFromFile(wPath, macrosRaw, nullptr, "main", "vs_5_0", 0, 0, &shaderData.blob, nullptr)))
	{
		return shaderData;
	}

	/*
	Create shader
	*/
	if (FAILED(m_device->CreateVertexShader(shaderData.blob->GetBufferPointer(), shaderData.blob->GetBufferSize(), nullptr, &shaderData.vs)))
	{
		return shaderData;
	}

	return shaderData;
}

ID3D11GeometryShader * ShaderCreatorV2::CompileAndCreateGeometryShader(const char* fileName, int nrOfDefines, ShaderDefine * defines)
{
	std::unique_ptr<_D3D_SHADER_MACRO[]> macros = nullptr;
	_D3D_SHADER_MACRO* macrosRaw = nullptr;
	ID3D10Blob* blob = nullptr;
	ID3D11GeometryShader* shader = nullptr;

	/*
	Convert file name
	*/
	std::optional<std::wstring> wName = StringConverter::ToWideString(fileName);
	if (!wName)
		return nullptr;

	/*
	Add path to file name
	*/
	wName = L"../Engine/FrameWork/HLSL/" + wName.value();
	const wchar_t* wPath = wName->c_str();

	if (nrOfDefines > 0)
	{
		/*
		Allocate one extra macro to null-terminate it
		*/
		macros = std::make_unique<_D3D_SHADER_MACRO[]>(nrOfDefines + 1U);
		macrosRaw = macros.get();

		/*
		Copy data
		*/
		for (int i = 0; i < nrOfDefines; i++)
		{
			macrosRaw[i].Name = defines[i].name;
			macrosRaw[i].Definition = defines[i].value;
		}


		/*
		The last macro must be null-terminated
		*/
		macrosRaw[nrOfDefines].Name = nullptr;
		macrosRaw[nrOfDefines].Definition = nullptr;
	}

	/*
	Compile shader
	*/
	if (FAILED(D3DCompileFromFile(wPath, macrosRaw, nullptr, "main", "gs_5_0", 0, 0, &blob, nullptr)))
	{
		return nullptr;
	}

	/*
	Create shader
	*/
	if (FAILED(m_device->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader)))
	{
		return nullptr;
	}
	blob->Release();

	return shader;
}

ID3D11PixelShader * ShaderCreatorV2::CompileAndCreatePixelShaderFromFile(const char* fileName, int nrOfDefines, ShaderDefine * defines)
{
	std::unique_ptr<_D3D_SHADER_MACRO[]> macros = nullptr;
	_D3D_SHADER_MACRO* macrosRaw = nullptr;
	ID3D10Blob* blob = nullptr;
	ID3D11PixelShader* shader = nullptr;

	/*
	Convert file name
	*/
	std::optional<std::wstring> wName = StringConverter::ToWideString(fileName);
	if (!wName)
		return nullptr;

	/*
	Add path to file name
	*/
	wName = L"../Engine/FrameWork/HLSL/" + wName.value();
	const wchar_t* wPath = wName->c_str();

	if (nrOfDefines > 0)
	{
		/*
		Allocate one extra macro to null-terminate it
		*/
		macros = std::make_unique<_D3D_SHADER_MACRO[]>(nrOfDefines + 1U);
		macrosRaw = macros.get();

		/*
		Copy data
		*/
		for (int i = 0; i < nrOfDefines; i++)
		{
			macrosRaw[i].Name = defines[i].name;
			macrosRaw[i].Definition = defines[i].value;
		}


		/*
		The last macro must be null-terminated
		*/
		macrosRaw[nrOfDefines].Name = nullptr;
		macrosRaw[nrOfDefines].Definition = nullptr;
	}

	/*
	Compile shader
	*/
	if (FAILED(D3DCompileFromFile(wPath, macrosRaw, nullptr, "main", "ps_5_0", 0, 0, &blob, nullptr)))
	{
		return nullptr;
	}

	/*
	Create shader
	*/
	if (FAILED(m_device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader)))
	{
		return nullptr;
	}
	blob->Release();

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