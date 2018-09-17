#include "InputLayoutCreator.hpp"
#include <d3d11.h>
#include <memory>

InputLayoutCreator::InputLayoutCreator() : m_device(nullptr)
{
}

InputLayoutCreator::~InputLayoutCreator()
{
}

bool InputLayoutCreator::Initialize(ID3D11Device * device)
{
	m_device = device;
	return true;
}

ID3D11InputLayout * InputLayoutCreator::CreateInputLayout(ID3D10Blob * vsBlob, int nrOfElements, InputElement * elements)
{
	ID3D11InputLayout* layout = nullptr;
	std::unique_ptr<D3D11_INPUT_ELEMENT_DESC[]> elems = nullptr;

	/*
	Ensure valid parameter values
	*/
	if (!vsBlob || !elements || nrOfElements <= 0)
	{
		return nullptr;
	}

	/*
	Allocate descriptions
	*/
	elems = std::make_unique<D3D11_INPUT_ELEMENT_DESC[]>(nrOfElements);

	/*
	Copy description data
	*/
	for (int i = 0; i < nrOfElements; i++)
	{
		/*
		Default values
		*/
		elems[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elems[i].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
		elems[i].InputSlot = 0;
		elems[i].InstanceDataStepRate = 0;
		elems[i].SemanticIndex = 0;

		/*
		Parameter values
		*/
		elems[i].SemanticName = elements[i].semanticName;
		switch (elements[i].format)
		{
		case ElementFormat::FLOAT1: elems[i].Format = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT; break;
		case ElementFormat::FLOAT2: elems[i].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT; break;
		case ElementFormat::FLOAT3: elems[i].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT; break;
		case ElementFormat::FLOAT4: elems[i].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT; break;
		default:
			return nullptr;
		}
	}

	/*
	Create layout
	*/
	if (FAILED(m_device->CreateInputLayout(elems.get(), nrOfElements, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &layout)))
	{
		return nullptr;
	}

	return layout;
}
