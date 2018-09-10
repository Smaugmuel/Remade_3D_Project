#include "ShaderManagerV2.hpp"
#include "../Misc/StringConverter.hpp"
#include <d3d11.h>
#include <d3dcompiler.h>

/*
The deferred rendering light pass shader needs to know the light limit when compiling
Whoever declares the constant buffer needs to know it
*/

ShaderManagerV2::ShaderManagerV2() : m_device(nullptr), m_deviceContext(nullptr)
{
}
ShaderManagerV2::~ShaderManagerV2()
{
}

bool ShaderManagerV2::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, int maxNrOfLights)
{
	m_device = device;
	m_deviceContext = deviceContext;

	VertexShaderData vsData;

	/*
	Define shader defines
	*/
	ShaderDefine vsDefines1[] =
	{
		{ "CBUFFER_HAS_VIEW_PROJECTION_MATRIX", nullptr },
		{ "CBUFFER_HAS_WORLD_MATRIX", nullptr },
		{ "VBUFFER_HAS_NORMAL", nullptr },
		{ "VBUFFER_HAS_UV", nullptr },
		{ "VBUFFER_PASS_WPOS", nullptr }
	};
	ShaderDefine vsDefines2[] =
	{
		{ "VBUFFER_HAS_UV", nullptr }
	};
	std::string nLightsStr = std::to_string(maxNrOfLights);
	ShaderDefine psDefines2[] =
	{
		{ "MAX_NR_OF_LIGHTS", nLightsStr.c_str() }
	};

	/*
	Define input layout elements
	*/
	InputElement vsElements1[] =
	{
		{ "POSITION", ElementFormat::FLOAT3 },
		{ "NORMAL", ElementFormat::FLOAT3 },
		{ "TEXCOORD", ElementFormat::FLOAT2 }
	};
	InputElement vsElements2[] =
	{
		{ "POSITION", ElementFormat::FLOAT3 },
		{ "TEXCOORD", ElementFormat::FLOAT2 }
	};

	const unsigned int nVSDefines1 = sizeof(vsDefines1) / sizeof(ShaderDefine);
	const unsigned int nVSDefines2 = sizeof(vsDefines2) / sizeof(ShaderDefine);
	const unsigned int nPSDefines2 = sizeof(psDefines2) / sizeof(ShaderDefine);
	const unsigned int nVSElements1 = sizeof(vsElements1) / sizeof(InputElement);
	const unsigned int nVSElements2 = sizeof(vsElements2) / sizeof(InputElement);

	const unsigned int gPassIndex = static_cast<unsigned int>(ShaderTypeV2::GEOMETRY_PASS);
	const unsigned int lPassIndex = static_cast<unsigned int>(ShaderTypeV2::LIGHT_PASS);
	const unsigned int sPassIndex = static_cast<unsigned int>(ShaderTypeV2::SHADOW_PASS);
	const unsigned int fPassIndex = static_cast<unsigned int>(ShaderTypeV2::FINAL_PASS);

	/*
	Initialize creator members
	*/
	if (!m_shaderCreator.Initialize(device))
		return false;
	if (!m_inputLayoutCreator.Initialize(device))
		return false;

	/* =============================== Geometry pass =============================================== */

	/*
	Create the vertex shader and input layout
	*/
	vsData = m_shaderCreator.CompileAndCreateVertexShader(nVSDefines1, vsDefines1);
	if (!vsData.blob || !vsData.vs)
		return false;
	m_vertexShaders[gPassIndex] = vsData.vs;
	m_inputLayouts[gPassIndex] = m_inputLayoutCreator.CreateInputLayout(vsData.blob, nVSElements1, vsElements1);
	if (!m_inputLayouts[gPassIndex])
		return false;
	vsData.blob->Release();
	
	/*
	Disable geometry shader
	*/
	m_geometryShaders[static_cast<unsigned int>(ShaderTypeV2::GEOMETRY_PASS)] = nullptr;

	/*
	Create the pixel shader
	*/
	m_pixelShaders[gPassIndex] = m_shaderCreator.CompileAndCreatePixelShaderFromFile("PS_GeometryPass.hlsl");
	if (!m_pixelShaders[gPassIndex])
		return false;

	/* =============================== Light pass =============================================== */

	/*
	Create the vertex shader and input layout
	*/
	vsData = m_shaderCreator.CompileAndCreateVertexShader(nVSDefines2, vsDefines2);
	if (!vsData.blob || !vsData.vs)
		return false;
	m_vertexShaders[lPassIndex] = vsData.vs;
	m_inputLayouts[lPassIndex] = m_inputLayoutCreator.CreateInputLayout(vsData.blob, nVSElements2, vsElements2);
	if (!m_inputLayouts[lPassIndex])
		return false;
	vsData.blob->Release();

	/*
	Disable geometry shader
	*/
	m_geometryShaders[static_cast<unsigned int>(ShaderTypeV2::LIGHT_PASS)] = nullptr;

	/*
	Create the pixel shader, with or without lights
	*/
	if (maxNrOfLights > 0)
		m_pixelShaders[lPassIndex] = m_shaderCreator.CompileAndCreatePixelShaderFromFile("PS_LightPass.hlsl", nPSDefines2, psDefines2);
	else
		m_pixelShaders[lPassIndex] = m_shaderCreator.CompileAndCreatePixelShaderFromFile("PS_LightPass.hlsl");
	if (!m_pixelShaders[lPassIndex])
		return false;

	/* =============================== Shadow pass =============================================== */

	/*
	Create the vertex shader and input layout
	*/
	m_vertexShaders[sPassIndex] = m_vertexShaders[lPassIndex];
	m_inputLayouts[sPassIndex] = m_inputLayouts[lPassIndex];

	/*
	Disable geometry shader
	*/
	m_geometryShaders[static_cast<unsigned int>(ShaderTypeV2::SHADOW_PASS)] = nullptr;

	/*
	Create the pixel shader, with or without lights
	*/
	if (maxNrOfLights > 0)
		m_pixelShaders[sPassIndex] = m_shaderCreator.CompileAndCreatePixelShaderFromFile("PS_ShadowPass.hlsl", nPSDefines2, psDefines2);
	else
		m_pixelShaders[sPassIndex] = m_shaderCreator.CompileAndCreatePixelShaderFromFile("PS_ShadowPass.hlsl");
	if (!m_pixelShaders[sPassIndex])
		return false;

	/* =============================== Final pass =============================================== */

	/*
	Create the vertex shader and input layout
	*/
	m_vertexShaders[fPassIndex] = m_vertexShaders[lPassIndex];
	m_inputLayouts[fPassIndex] = m_inputLayouts[lPassIndex];

	/*
	Disable geometry shader
	*/
	m_geometryShaders[static_cast<unsigned int>(ShaderTypeV2::FINAL_PASS)] = nullptr;

	/*
	Create the pixel shader
	*/
	m_pixelShaders[fPassIndex] = m_shaderCreator.CompileAndCreatePixelShaderFromFile("PS_FinalPass.hlsl");
	if (!m_pixelShaders[fPassIndex])
		return false;

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