#include "ShaderStorage.hpp"
#include <d3d11.h>
#include <d3dcompiler.h>
#include "StringConverter.hpp"

ShaderStorage* Singleton<ShaderStorage>::s_instance = nullptr;

ShaderStorage::ShaderStorage()
{
	InputDescription* desc;

	/* ================================ Create input layout descriptions ================================ */

	// Position, Normal, Color
	desc = &m_inputDescriptions["PNC"];
	desc->n = 3;
	desc->elements = new D3D11_INPUT_ELEMENT_DESC[3];
	desc->elements[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	desc->elements[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	desc->elements[2] = { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };

	// Position, Normal, UV
	desc = &m_inputDescriptions["PNU"];
	desc->n = 3;
	desc->elements = new D3D11_INPUT_ELEMENT_DESC[3];
	desc->elements[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	desc->elements[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	desc->elements[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };

	// Position, UV
	desc = &m_inputDescriptions["PU"];
	desc->n = 2;
	desc->elements = new D3D11_INPUT_ELEMENT_DESC[2];
	desc->elements[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	desc->elements[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };

	// Position
	desc = &m_inputDescriptions["P"];
	desc->n = 1;
	desc->elements = new D3D11_INPUT_ELEMENT_DESC[1];
	desc->elements[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };

	/* =================================== Set up input layout linker =================================== */
	m_linker["VS_Texture.hlsl"] = "PNU";
	m_linker["VS_D_Texture.hlsl"] = "PNU";
	m_linker["VS_PosNormColor.hlsl"] = "PNC";
	m_linker["VS_PosUV.hlsl"] = "PU";
	m_linker["VS_Depth.hlsl"] = "P";
	m_linker["VS_D_TextureChunk.hlsl"] = "PNU";
}
ShaderStorage::~ShaderStorage()
{
	for (auto a : m_inputDescriptions)
	{
		delete[] a.second.elements;
	}
}

bool ShaderStorage::CreateVertexShader(ID3D11Device * device, std::string name)
{
	if (HasVertexShader(name))
		return true;

	ID3D10Blob* blob;
	HRESULT result;
	std::wstring wstr = ToWideString(name);

	result = D3DCompileFromFile(
		wstr.c_str(),
		nullptr,
		nullptr,
		"main",
		"vs_5_0",
		0,
		0,
		&blob,
		nullptr
	);
	if (FAILED(result))
		return false;

	result = device->CreateVertexShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		nullptr,
		&m_vertexShaders[name]
	);
	if (FAILED(result))
		return false;

	
	std::string linkedName = m_linker[name];
	InputDescription desc = m_inputDescriptions[linkedName];

	result = device->CreateInputLayout(
		desc.elements,
		desc.n,
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		&m_inputLayouts[linkedName]
	);
	if (FAILED(result))
		return false;

	return true;
}
bool ShaderStorage::CreateGeometryShader(ID3D11Device * device, std::string name)
{
	if (HasGeometryShader(name) || name == "NULL")
		return true;

	ID3D10Blob* blob;
	HRESULT result;
	std::wstring wstr = ToWideString(name);

	result = D3DCompileFromFile(
		wstr.c_str(),
		nullptr,
		nullptr,
		"main",
		"gs_5_0",
		0,
		0,
		&blob,
		nullptr
	);
	if (FAILED(result))
		return false;

	result = device->CreateGeometryShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		nullptr,
		&m_geometryShaders[name]
	);
	if (FAILED(result))
		return false;

	return true;
}
bool ShaderStorage::CreatePixelShader(ID3D11Device * device, std::string name)
{
	if (HasPixelShader(name))
		return true;

	ID3D10Blob* blob;
	HRESULT result;
	std::wstring wstr = ToWideString(name);

	result = D3DCompileFromFile(
		wstr.c_str(),
		nullptr,
		nullptr,
		"main",
		"ps_5_0",
		0,
		0,
		&blob,
		nullptr
	);
	if (FAILED(result))
		return false;

	result = device->CreatePixelShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		nullptr,
		&m_pixelShaders[name]
	);
	if (FAILED(result))
		return false;

	return true;
}

ID3D11VertexShader * ShaderStorage::GetVertexShader(std::string name)
{
	/*if (!HasVertexShader(name))
		return nullptr;*/

	return m_vertexShaders.at(name);
}
ID3D11GeometryShader * ShaderStorage::GetGeometryShader(std::string name)
{
	/*if (!HasGeometryShader(name))
		return nullptr;*/

	if (name == "NULL")
		return nullptr;

	return m_geometryShaders.at(name);
}
ID3D11PixelShader * ShaderStorage::GetPixelShader(std::string name)
{
	/*if (!HasPixelShader(name))
		return nullptr;*/

	return m_pixelShaders.at(name);
}

ID3D11InputLayout * ShaderStorage::GetInputLayout(std::string name)
{
	/*LinkerMap::iterator it = m_linker.find(name);
	if (it == m_linker.end())
		return nullptr;

	LayoutMap::iterator it2 = m_layouts.find(it->second);
	if (it2 == m_layouts.end())
		return nullptr;

	return it2->second;*/

	return m_inputLayouts.at(m_linker.at(name));
}

bool ShaderStorage::HasVertexShader(std::string name) const
{
	return m_vertexShaders.find(name) != m_vertexShaders.end();
}
bool ShaderStorage::HasGeometryShader(std::string name) const
{
	return m_geometryShaders.find(name) != m_geometryShaders.end();
}
bool ShaderStorage::HasPixelShader(std::string name) const
{
	return m_pixelShaders.find(name) != m_pixelShaders.end();
}

unsigned int ShaderStorage::NrOfVertexShaders() const
{
	return static_cast<unsigned int>(m_vertexShaders.size());
}
unsigned int ShaderStorage::NrOfGeometryShaders() const
{
	return static_cast<unsigned int>(m_geometryShaders.size());
}
unsigned int ShaderStorage::NrOfPixelShaders() const
{
	return static_cast<unsigned int>(m_pixelShaders.size());
}