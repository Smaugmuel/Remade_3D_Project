#include "ShaderManagerV2.hpp"
#include "../Misc/StringConverter.hpp"
#include <d3d11.h>
#include <d3dcompiler.h>

ShaderManagerV2::ShaderManagerV2() : m_device(nullptr), m_deviceContext(nullptr)
{
}
ShaderManagerV2::~ShaderManagerV2()
{
}

bool ShaderManagerV2::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext)
{
	m_device = device;
	m_deviceContext = deviceContext;

	ID3D10Blob* blob;
	HRESULT result;

	D3D11_INPUT_ELEMENT_DESC firstDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	D3D11_INPUT_ELEMENT_DESC lightDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	
	/*
	Create the vertex shader and input layout for the first pass
	*/
	result = D3DCompileFromFile(L"../Engine/FrameWork/HLSL/VS_FirstPass.hlsl", nullptr, nullptr, "main", "vs_5_0", 0, 0, &blob, nullptr);
	if (FAILED(result))
		return false;
	result = m_device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_vertexShaders[static_cast<unsigned int>(ShaderTypeV2::FIRST_PASS)]);
	if (FAILED(result))
		return false;
	result = m_device->CreateInputLayout(firstDesc, sizeof(firstDesc) / sizeof(firstDesc[0]), blob->GetBufferPointer(), blob->GetBufferSize(), &m_inputLayouts[static_cast<unsigned int>(ShaderTypeV2::FIRST_PASS)]);
	if (FAILED(result))
		return false;
	blob->Release();

	/*
	Create the pixel shader for the first pass
	*/
	result = D3DCompileFromFile(L"../Engine/FrameWork/HLSL/PS_FirstPass.hlsl", nullptr, nullptr, "main", "ps_5_0", 0, 0, &blob, nullptr);
	if (FAILED(result))
		return false;
	result = m_device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_pixelShaders[static_cast<unsigned int>(ShaderTypeV2::FIRST_PASS)]);
	if (FAILED(result))
		return false;
	blob->Release();

	/*
	Create the vertex shader and input layout for the light pass
	*/
	result = D3DCompileFromFile(L"../Engine/FrameWork/HLSL/VS_LightPass.hlsl", nullptr, nullptr, "main", "vs_5_0", 0, 0, &blob, nullptr);
	if (FAILED(result))
		return false;
	result = m_device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_vertexShaders[static_cast<unsigned int>(ShaderTypeV2::LIGHT_PASS)]);
	if (FAILED(result))
		return false;
	result = m_device->CreateInputLayout(lightDesc, sizeof(lightDesc) / sizeof(lightDesc[0]), blob->GetBufferPointer(), blob->GetBufferSize(), &m_inputLayouts[static_cast<unsigned int>(ShaderTypeV2::LIGHT_PASS)]);
	if (FAILED(result))
		return false;
	blob->Release();

	/*
	Create the pixel shader for the light pass
	*/
	result = D3DCompileFromFile(L"../Engine/FrameWork/HLSL/PS_LightPass.hlsl", nullptr, nullptr, "main", "ps_5_0", 0, 0, &blob, nullptr);
	if (FAILED(result))
		return false;
	result = m_device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_pixelShaders[static_cast<unsigned int>(ShaderTypeV2::LIGHT_PASS)]);
	if (FAILED(result))
		return false;
	blob->Release();

	/*
	Disable geometry shaders for these passes
	*/
	m_geometryShaders[static_cast<unsigned int>(ShaderTypeV2::FIRST_PASS)] = nullptr;
	m_geometryShaders[static_cast<unsigned int>(ShaderTypeV2::LIGHT_PASS)] = nullptr;

	return true;
}

bool ShaderManagerV2::SetShaders(ShaderTypeV2 shaderType)
{
	m_deviceContext->VSSetShader(m_vertexShaders[static_cast<unsigned int>(shaderType)], nullptr, 0);
	m_deviceContext->GSSetShader(m_geometryShaders[static_cast<unsigned int>(shaderType)], nullptr, 0);
	m_deviceContext->PSSetShader(m_pixelShaders[static_cast<unsigned int>(shaderType)], nullptr, 0);

	m_deviceContext->IASetInputLayout(m_inputLayouts[static_cast<unsigned int>(shaderType)]);

	return true;
}

/*int ShaderManagerV2::CreateVertexShader(unsigned int flags)
{
	// Define more macros than neccessary, since a null-termination determines the amount
	D3D_SHADER_MACRO macros[4];

	if (flags & static_cast<unsigned int>(VSFlags::RECEIVE_NORMAL))
	{
		if (flags & static_cast<unsigned int>(VSFlags::RECEIVE_UV))
		{
			if (flags & static_cast<unsigned int>(VSFlags::PASS_WPOS))
			{
				macros[0] = { "RECEIVE_NORMAL", "" };
				macros[1] = { "RECEIVE_UV", "" };
				macros[2] = { "PASS_WPOS", "" };
				macros[3] = { 0, 0 };
			}
			else
			{
				macros[0] = { "RECEIVE_NORMAL", "" };
				macros[1] = { "RECEIVE_UV", "" };
				macros[2] = { 0, 0 };
			}
		}
		else
		{
			if (flags & static_cast<unsigned int>(VSFlags::PASS_WPOS))
			{
				macros[0] = { "RECEIVE_NORMAL", "" };
				macros[1] = { "PASS_WPOS", "" };
				macros[2] = { 0, 0 };
			}
			else
			{
				macros[0] = { "RECEIVE_NORMAL", "" };
				macros[1] = { 0, 0 };
			}
		}
	}
	else
	{
		if (flags & static_cast<unsigned int>(VSFlags::RECEIVE_UV))
		{
			if (flags & static_cast<unsigned int>(VSFlags::PASS_WPOS))
			{
				macros[0] = { "RECEIVE_UV", "" };
				macros[1] = { "PASS_WPOS", "" };
				macros[2] = { 0, 0 };
			}
			else
			{
				macros[0] = { "RECEIVE_UV", "" };
				macros[1] = { 0, 0 };
			}
		}
		else
		{
			if (flags & static_cast<unsigned int>(VSFlags::PASS_WPOS))
			{
				macros[0] = { "PASS_WPOS", "" };
				macros[1] = { 0, 0 };
			}
			else
			{
				macros[0] = { 0, 0 };
			}
		}
	}



	return 0;
}

int ShaderManagerV2::CreateVertexShader(const std::string & fileName)
{
	ID3D10Blob* blob;
	ID3D11VertexShader* shader;
	HRESULT result;

	std::optional<std::wstring> wstr = StringConverter::ToWideString("../Render/Shaders/HLSL/" + fileName);
	if (!wstr)
		return -1;

	result = D3DCompileFromFile(wstr.value().c_str(), nullptr, nullptr, "main", "vs_5_0", 0, 0, &blob, nullptr);
	if (FAILED(result))
		return -1;

	result = m_device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);
	if (FAILED(result))
		return -1;

	m_vertexShaders.push_back(shader);
	return m_vertexShaders.size() - 1;
}
int ShaderManagerV2::CreateGeometryShader(const std::string & fileName)
{
	ID3D10Blob* blob;
	ID3D11GeometryShader* shader;
	HRESULT result;

	std::optional<std::wstring> wstr = StringConverter::ToWideString("../Render/Shaders/HLSL/" + fileName);
	if (!wstr)
		return -1;

	result = D3DCompileFromFile(wstr.value().c_str(), nullptr, nullptr, "main", "gs_5_0", 0, 0, &blob, nullptr);
	if (FAILED(result))
		return -1;

	result = m_device->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);
	if (FAILED(result))
		return -1;

	m_geometryShaders.push_back(shader);
	return m_geometryShaders.size() - 1;
}
int ShaderManagerV2::CreatePixelShader(const std::string & fileName)
{
	ID3D10Blob* blob;
	ID3D11PixelShader* shader;
	HRESULT result;

	std::optional<std::wstring> wstr = StringConverter::ToWideString("../Render/Shaders/HLSL/" + fileName);
	if (!wstr)
		return -1;

	result = D3DCompileFromFile(wstr.value().c_str(), nullptr, nullptr, "main", "ps_5_0", 0, 0, &blob, nullptr);
	if (FAILED(result))
		return -1;

	result = m_device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);
	if (FAILED(result))
		return -1;

	m_pixelShaders.push_back(shader);
	return m_pixelShaders.size() - 1;
}*/

//bool ShaderManagerV2::SetVertexShaderToPipeline(int id)
//{
//	if (!ValidVSID(id))
//		return false;
//
//	m_deviceContext->VSSetShader(m_vertexShaders[id], nullptr, 0);
//	return true;
//}
//bool ShaderManagerV2::SetGeometryShaderToPipeline(int id)
//{
//	if (!ValidGSID(id))
//		return false;
//
//	m_deviceContext->GSSetShader(m_geometryShaders[id], nullptr, 0);
//	return true;
//}
//bool ShaderManagerV2::SetPixelShaderToPipeline(int id)
//{
//	if (!ValidPSID(id))
//		return false;
//
//	m_deviceContext->PSSetShader(m_pixelShaders[id], nullptr, 0);
//	return true;
//}
//
//bool ShaderManagerV2::ValidVSID(int id)
//{
//	return id >= 0 && id < static_cast<int>(m_vertexShaders.size());
//}
//bool ShaderManagerV2::ValidGSID(int id)
//{
//	return id >= 0 && id < static_cast<int>(m_geometryShaders.size());
//}
//bool ShaderManagerV2::ValidPSID(int id)
//{
//	return id >= 0 && id < static_cast<int>(m_pixelShaders.size());
//}