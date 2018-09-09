#include "ShaderCreator.hpp"
#include <d3d11.h>
#include <d3dcompiler.h>
#include "../Misc/StringConverter.hpp"

ShaderCreator::ShaderCreator() :
	m_device(nullptr), m_deviceContext(nullptr)
{
}

ShaderCreator::~ShaderCreator()
{
}

bool ShaderCreator::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext)
{
	m_device = device;
	m_deviceContext = deviceContext;

	return true;
}

int ShaderCreator::CreateInputLayoutFromVertexShader(int vsIndex, unsigned int nrOfInputElements, D3D11_INPUT_ELEMENT_DESC * inputElements)
{
	ID3D10Blob* blob = nullptr;
	ID3D11InputLayout* layout = nullptr;

	/*
	Check if index is valid
	*/
	if (vsIndex < 0 || vsIndex >= m_vs.size())
		return -1;

	/*
	Retrieve blob
	*/
	blob = m_vsBlobs[vsIndex];

	/*
	Create layout from blob and elements
	*/
	if (FAILED(m_device->CreateInputLayout(inputElements, nrOfInputElements, blob->GetBufferPointer(), blob->GetBufferSize(), &layout)))
		return -1;

	m_layouts.push_back(layout);
	return m_layouts.size() - 1;
}

int ShaderCreator::CreateVertexShader(const std::string & name, D3D_SHADER_MACRO * macros)
{
	if (m_vsNameLinker.find(name) != m_vsNameLinker.end())
	{
		return m_vsNameLinker.at(name);
	}

	ID3D10Blob* blob = nullptr;
	ID3D11VertexShader* shader = nullptr;
	
	std::optional<std::wstring> wName = StringConverter::ToWideString(name);
	if (!wName)
		return -1;

	std::wstring fullName = L"../Engine/FrameWork/HLSL/" + wName.value();

	if (FAILED(D3DCompileFromFile(fullName.c_str(), macros, nullptr, "main", "vs_5_0", 0, 0, &blob, nullptr)))
		return -1;
	if (FAILED(m_device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader)))
		return -1;

	m_vsBlobs.push_back(blob);
	m_vs.push_back(shader);

	return m_vs.size() - 1;
}

int ShaderCreator::CreateHullShader(const std::string & name, D3D_SHADER_MACRO * macros)
{
	return 0;
}

int ShaderCreator::CreateDomainShader(const std::string & name, D3D_SHADER_MACRO * macros)
{
	return 0;
}

int ShaderCreator::CreateGeometryShader(const std::string & name, D3D_SHADER_MACRO * macros)
{
	return 0;
}

int ShaderCreator::CreatePixelShader(const std::string & name, D3D_SHADER_MACRO * macros)
{
	return 0;
}
