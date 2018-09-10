#include "TextureManager.hpp"
#include "../../packages/directxtk_desktop_2015.2018.7.3.1/include/WICTextureLoader.h"
#include "../Misc/StringConverter.hpp"
#include <d3d11.h>

TextureManager::TextureManager() : m_device(nullptr), m_deviceContext(nullptr)
{
}

TextureManager::~TextureManager()
{
	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		m_textures[i]->Release();
	}
}

bool TextureManager::Initialize(ID3D11Device * device, ID3D11DeviceContext* deviceContext)
{
	m_device = device;
	m_deviceContext = deviceContext;
	return true;
}

int TextureManager::LoadTexture(const std::string & fileName)
{
	/*
	Check if this texture has already been loaded
	*/
	if (m_nameToIndexLinker.find(fileName) != m_nameToIndexLinker.end())
	{
		return m_nameToIndexLinker.at(fileName);
	}

	m_textures.data();

	/*
	Convert to wide string
	*/
	std::optional<std::wstring> wstr = StringConverter::ToWideString("../Textures/" + std::string(fileName));
	if (!wstr)
		return -1;

	/*
	Load texture
	*/
	ID3D11ShaderResourceView* resource = nullptr;
	if (FAILED(DirectX::CreateWICTextureFromFile(m_device, wstr.value().c_str(), nullptr, &resource)))
		return -1;

	/*
	Link the file name to the index
	*/
	m_textures.push_back(resource);
	int index = m_textures.size() - 1;
	m_nameToIndexLinker[fileName] = index;

	return index;
}

bool TextureManager::SetTextureToPixelShader(int id, int slot)
{
	if (id < 0 || id >= static_cast<int>(m_textures.size()))
		return false;
	
	m_deviceContext->PSSetShaderResources(slot, 1, &m_textures[id]);
	return true;
}

int TextureManager::GetTextureIndex(const std::string & fileName)
{
	if (m_nameToIndexLinker.find(fileName) != m_nameToIndexLinker.end())
	{
		return m_nameToIndexLinker.at(fileName);
	}
	return -1;
}

ID3D11ShaderResourceView * TextureManager::GetTexture(int id)
{
	return id >= 0 && id < static_cast<int>(m_textures.size()) ? m_textures[id] : nullptr;
}