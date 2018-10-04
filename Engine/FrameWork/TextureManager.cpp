#include "TextureManager.hpp"
#include "../../packages/directxtk_desktop_2015.2018.7.3.1/include/WICTextureLoader.h"
#include "../Misc/StringConverter.hpp"
#include <d3d11.h>

TextureManager::TextureManager() : m_device(nullptr), m_deviceContext(nullptr), m_nrOfTextures(0U)
{
}

TextureManager::~TextureManager()
{
	for (unsigned int i = 0; i < m_nrOfTextures; i++)
	{
		if (m_textures[i])
		{
			m_textures[i]->Release();
		}
	}
}

bool TextureManager::Initialize(ID3D11Device * device, ID3D11DeviceContext* deviceContext)
{
	m_device = device;
	m_deviceContext = deviceContext;

	for (unsigned int i = 0; i < MAX_NR_OF_TEXTURES; i++)
	{
		m_textures[i] = nullptr;
		m_textureNames[i] = "";
	}
	return true;
}

int TextureManager::LoadTexture(const std::string & fileName)
{
	/*
	Check if this texture has already been loaded
	*/
	int textureIndex = GetTextureIndex(fileName);
	if (textureIndex != -1)
		return textureIndex;

	/*
	Convert name to wide string
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

	m_textures[m_nrOfTextures] = resource;
	m_textureNames[m_nrOfTextures] = fileName;

	return m_nrOfTextures++;
}

bool TextureManager::SetTextureToPixelShader(int index, int slot)
{
	if (index < 0 || index >= static_cast<int>(m_nrOfTextures))
		return false;
	
	m_deviceContext->PSSetShaderResources(slot, 1, &m_textures[index]);
	return true;
}

int TextureManager::GetTextureIndex(const std::string & fileName)
{
	for (unsigned int i = 0; i < m_nrOfTextures; i++)
	{
		if (m_textureNames[i] == fileName)
		{
			return i;
		}
	}
	return -1;
}

ID3D11ShaderResourceView * TextureManager::GetTexture(int index)
{
	return index >= 0 && index < static_cast<int>(m_nrOfTextures) ? m_textures[index] : nullptr;
}