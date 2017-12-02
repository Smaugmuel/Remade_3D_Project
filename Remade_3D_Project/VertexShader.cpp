#include "VertexShader.hpp"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include "Buffers.hpp"

#include "Object.hpp"
#include "Camera.hpp"

VertexShader::VertexShader()
{
}
VertexShader::~VertexShader()
{
	if (m_inputLayout)
	{
		m_inputLayout->Release();
		m_inputLayout = nullptr;
	}
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = nullptr;
	}
	if (m_perFrameConstantBuffer)
	{
		m_perFrameConstantBuffer->Release();
		m_perFrameConstantBuffer = nullptr;
	}
	if (m_perObjectConstantBuffer)
	{
		m_perObjectConstantBuffer->Release();
		m_perObjectConstantBuffer = nullptr;
	}
}

bool VertexShader::Initialize(ID3D11Device* device, const char* shaderType)
{
	ID3D10Blob* vertexShaderBlob;
	HRESULT result;

	std::string str((std::string)"VS_" + shaderType + ".hlsl");	// Add prefix and suffix
	std::wstring wstr(str.size(), L' ');						// Set size (has to be done) and add the L
	std::copy(str.begin(), str.end(), wstr.begin());			// Copy to wstring

	result = D3DCompileFromFile(
		wstr.c_str(),
		nullptr,
		nullptr,
		"main",
		"vs_5_0",
		0,
		0,
		&vertexShaderBlob,
		nullptr
		);
	if (FAILED(result))
	{
		return false;
	}

	result = device->CreateVertexShader(
		vertexShaderBlob->GetBufferPointer(),
		vertexShaderBlob->GetBufferSize(),
		nullptr,
		&m_vertexShader
		);
	if (FAILED(result))
	{
		return false;
	}

	if (!InitializeInputLayout(device, vertexShaderBlob, shaderType))
	{
		return false;
	}

	vertexShaderBlob->Release();
	vertexShaderBlob = nullptr;


	if (!InitializeConstantBuffers(device))
	{
		return false;
	}
	
	return true;
}

void VertexShader::SetPerFrameConstantBuffer(ID3D11DeviceContext* deviceContext, Camera* camera)
{
	//D3D11_MAPPED_SUBRESOURCE mappedResource;
	//HRESULT result;
	//BufferType* data;
	//
	//// Mapping and updating PerFrameConstantBuffer
	//result = deviceContext->Map(
	//	m_perFrameConstantBuffer,
	//	0,
	//	D3D11_MAP_WRITE_DISCARD,
	//	0,
	//	&mappedResource
	//	);
	//if (FAILED(result))
	//{
	//	return;
	//}
	//
	//data = (BufferType*)mappedResource.pData;
	//data->SetValues(args...);
	//
	//deviceContext->VSSetConstantBuffers(0, 1, &m_perFrameConstantBuffer);
	//deviceContext->Unmap(m_perFrameConstantBuffer, 0);
	



	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result;
	PerFrameConstantBuffer* pfcbData;

	// Mapping and updating PerFrameConstantBuffer
	result = deviceContext->Map(
		m_perFrameConstantBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource
		);
	if (FAILED(result))
	{
		return;
	}

	pfcbData = (PerFrameConstantBuffer*)mappedResource.pData;
	pfcbData->view = camera->GetViewMatrix();
	pfcbData->projection = camera->GetProjectionMatrix();

	deviceContext->VSSetConstantBuffers(0, 1, &m_perFrameConstantBuffer);
	deviceContext->Unmap(m_perFrameConstantBuffer, 0);
}
void VertexShader::SetPerObjectConstantBuffer(ID3D11DeviceContext* deviceContext, Object* object)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result;
	PerObjectConstantBuffer* pocbData;

	// Mapping and updating PerObjectConstantBuffer
	result = deviceContext->Map(
		m_perObjectConstantBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource
		);
	if (FAILED(result))
	{
		return;
	}

	pocbData = (PerObjectConstantBuffer*)mappedResource.pData;
	pocbData->world = object->GetWorldMatrix();

	deviceContext->VSSetConstantBuffers(1, 1, &m_perObjectConstantBuffer);
	deviceContext->Unmap(m_perObjectConstantBuffer, 0);
}

bool VertexShader::InitializeInputLayout(ID3D11Device* device, ID3D10Blob* vertexShaderBuffer, const char* shaderType)
{
	HRESULT result;
	std::string str(shaderType);

	if (str == "SingleColor")
	{
		D3D11_INPUT_ELEMENT_DESC inputDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		result = device->CreateInputLayout(
			inputDesc,
			ARRAYSIZE(inputDesc),
			vertexShaderBuffer->GetBufferPointer(),
			vertexShaderBuffer->GetBufferSize(),
			&m_inputLayout
			);
		if (FAILED(result))
		{
			return false;
		}

		return true;
	}

	return false;
}
bool VertexShader::InitializeConstantBuffers(ID3D11Device* device)
{
	m_perObjectConstantBuffer = Buffers::CreateConstantBuffer(device, sizeof(PerObjectConstantBuffer));
	if (!m_perObjectConstantBuffer)
	{
		return false;
	}

	m_perFrameConstantBuffer = Buffers::CreateConstantBuffer(device, sizeof(PerFrameConstantBuffer));	
	if (!m_perFrameConstantBuffer)
	{
		return false;
	}

	return true;
}

ID3D11VertexShader* VertexShader::GetShader() const
{
	return m_vertexShader;
}

ID3D11InputLayout* VertexShader::GetInputLayout() const
{
	return m_inputLayout;
}

ID3D11Buffer* VertexShader::GetPerFrameConstantBuffer() const
{
	return m_perFrameConstantBuffer;
}
ID3D11Buffer* VertexShader::GetPerObjectConstantBuffer() const
{
	return m_perObjectConstantBuffer;
}