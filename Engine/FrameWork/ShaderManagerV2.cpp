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

bool ShaderManagerV2::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, int maxNrOfLights, int nrOfWorldMatricesInBuffer)
{
	m_device = device;
	m_deviceContext = deviceContext;

	VertexShaderData vsData;

	/*
	Define shader defines
	*/
	std::string nMatsStr = std::to_string(nrOfWorldMatricesInBuffer);
	const char* nMatsCh = nMatsStr.c_str();
	std::string nInstances = std::to_string(28);
	const char* nInstancesCh = nInstances.c_str();

	ShaderDefine vsDefinesGPass[] =
	{
		{ "CBUFFER_VIEW_PROJECTION_MATRIX", nullptr },
		{ "CBUFFER_NR_OF_MATRICES_PER_BUFFER", nMatsCh },
		{ "VBUFFER_NORMAL", nullptr },
		{ "VBUFFER_UV", nullptr },
		{ "VBUFFER_PASS_WPOS", nullptr }
	};
	ShaderDefine vsDefinesGIPass[] =
	{
		{ "PASS_TO_GEOMETRY_SHADER", nullptr },
		{ "VBUFFER_NORMAL", nullptr },
		{ "VBUFFER_UV", nullptr }
	};
	ShaderDefine vsDefinesLPass[] =
	{
		{ "VBUFFER_UV", nullptr }
	};

	/*ShaderDefine gsDefinesGIPass[] =
	{
		{ "CBUFFER_VIEW_PROJECTION_MATRIX", nullptr },
		{ "CBUFFER_NR_OF_MATRICES_PER_BUFFER", nMatsCh },
		{ "VBUFFER_PASS_WPOS", nullptr },
		{ "VBUFFER_NORMAL", nullptr },
		{ "VBUFFER_UV", nullptr },
		{ "MAX_INSTANCING_AMOUNT", nInstancesCh }
	};*/

	std::string nLightsStr = std::to_string(maxNrOfLights);
	ShaderDefine psDefinesLPass[] =
	{
		{ "CBUFFER_MAX_NR_OF_LIGHTS", nLightsStr.c_str() }
	};

	/*
	Define input layout elements
	*/
	InputElement vsElementsGPass[] =
	{
		{ "POSITION", ElementFormat::FLOAT3 },
		{ "NORMAL", ElementFormat::FLOAT3 },
		{ "TEXCOORD", ElementFormat::FLOAT2 }
	};
	InputElement vsElementsLPass[] =
	{
		{ "POSITION", ElementFormat::FLOAT3 },
		{ "TEXCOORD", ElementFormat::FLOAT2 }
	};

	const unsigned int nVSDefinesGPass = sizeof(vsDefinesGPass) / sizeof(ShaderDefine);
	const unsigned int nVSDefinesGIPass = sizeof(vsDefinesGIPass) / sizeof(ShaderDefine);
	const unsigned int nVSDefinesLPass = sizeof(vsDefinesLPass) / sizeof(ShaderDefine);
	//const unsigned int nGSDefinesGIPass = sizeof(gsDefinesGIPass) / sizeof(ShaderDefine);
	const unsigned int nPSDefinesLPass = sizeof(psDefinesLPass) / sizeof(ShaderDefine);
	const unsigned int nVSElementsGPass = sizeof(vsElementsGPass) / sizeof(InputElement);
	const unsigned int nVSElementsLPass = sizeof(vsElementsLPass) / sizeof(InputElement);

	const unsigned int gPassIndex = static_cast<unsigned int>(ShaderTypeV2::GEOMETRY_PASS);
	const unsigned int giPassIndex = static_cast<unsigned int>(ShaderTypeV2::INSTANCED_GEOMETRY_PASS);
	const unsigned int sPassIndex = static_cast<unsigned int>(ShaderTypeV2::SHADOW_PASS);
	const unsigned int lPassIndex = static_cast<unsigned int>(ShaderTypeV2::LIGHT_PASS);
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
	vsData = m_shaderCreator.CompileAndCreateVertexShader(nVSDefinesGPass, vsDefinesGPass);
	if (!vsData.blob || !vsData.vs)
		return false;
	m_vertexShaders[gPassIndex] = vsData.vs;
	m_inputLayouts[gPassIndex] = m_inputLayoutCreator.CreateInputLayout(vsData.blob, nVSElementsGPass, vsElementsGPass);
	if (!m_inputLayouts[gPassIndex])
		return false;
	vsData.blob->Release();
	
	/*
	Disable geometry shader
	*/
	m_geometryShaders[gPassIndex] = nullptr;

	/*
	Create the pixel shader
	*/
	m_pixelShaders[gPassIndex] = m_shaderCreator.CompileAndCreatePixelShaderFromFile("PS_GeometryPass.hlsl");
	if (!m_pixelShaders[gPassIndex])
		return false;

	/* =============================== Geometry instancing pass =============================================== */

	/*
	Create the vertex shader and input layout
	*/
	vsData = m_shaderCreator.CompileAndCreateVertexShader(nVSDefinesGIPass, vsDefinesGIPass);
	if (!vsData.blob || !vsData.vs)
		return false;
	m_vertexShaders[giPassIndex] = vsData.vs;
	m_inputLayouts[giPassIndex] = m_inputLayoutCreator.CreateInputLayout(vsData.blob, nVSElementsGPass, vsElementsGPass);
	if (!m_inputLayouts[giPassIndex])
		return false;
	vsData.blob->Release();

	/*
	Create the geometry shader
	*/
	//m_geometryShaders[giPassIndex] = m_shaderCreator.CompileAndCreateGeometryShaderFromFile("GS_Instancing.hlsl", nGSDefinesGIPass, gsDefinesGIPass);
	m_geometryShaders[giPassIndex] = m_shaderCreator.CompileAndCreateGeometryShaderFromFile("GS_Instancing.hlsl");
	if (!m_geometryShaders[giPassIndex])
		return false;

	/*
	Create the pixel shader
	*/
	m_pixelShaders[giPassIndex] = m_pixelShaders[gPassIndex];
	if (!m_pixelShaders[giPassIndex])
		return false;

	/* =============================== Light pass =============================================== */

	/*
	Create the vertex shader and input layout
	*/
	vsData = m_shaderCreator.CompileAndCreateVertexShader(nVSDefinesLPass, vsDefinesLPass);
	if (!vsData.blob || !vsData.vs)
		return false;
	m_vertexShaders[lPassIndex] = vsData.vs;
	m_inputLayouts[lPassIndex] = m_inputLayoutCreator.CreateInputLayout(vsData.blob, nVSElementsLPass, vsElementsLPass);
	if (!m_inputLayouts[lPassIndex])
		return false;
	vsData.blob->Release();

	/*
	Disable geometry shader
	*/
	m_geometryShaders[lPassIndex] = nullptr;

	/*
	Create the pixel shader, with or without lights
	*/
	if (maxNrOfLights > 0)
		m_pixelShaders[lPassIndex] = m_shaderCreator.CompileAndCreatePixelShaderFromFile("PS_LightPass.hlsl", nPSDefinesLPass, psDefinesLPass);
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
	m_geometryShaders[sPassIndex] = nullptr;

	/*
	Create the pixel shader, with or without lights
	*/
	if (maxNrOfLights > 0)
		m_pixelShaders[sPassIndex] = m_shaderCreator.CompileAndCreatePixelShaderFromFile("PS_ShadowPass.hlsl", nPSDefinesLPass, psDefinesLPass);
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
	m_geometryShaders[fPassIndex] = nullptr;

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