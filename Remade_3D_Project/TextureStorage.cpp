#include "TextureStorage.hpp"
#include "Texture.hpp"

TextureStorage* Singleton<TextureStorage>::s_instance = nullptr;

TextureStorage::TextureStorage()
{
}

TextureStorage::~TextureStorage()
{
}

bool TextureStorage::LoadTexture(ID3D11Device * device, const std::string & name)
{
	if (HasTexture(name))
	{
		return true;
	}

	m_textures[name] = std::make_unique<Texture>();

	if (!m_textures[name]->Initialize(device, name.c_str()))
		return false;

	return true;
}

ID3D11ShaderResourceView * TextureStorage::GetTexture(const std::string & name)
{
	/*if (!HasTexture(name))
	{
		return nullptr;
	}*/
	return m_textures[name]->GetShaderResourceView();
}

bool TextureStorage::HasTexture(const std::string & name) const
{
	return m_textures.find(name) != m_textures.end();
}
