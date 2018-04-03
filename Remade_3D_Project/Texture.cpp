#include "Texture.hpp"
#include <WICTextureLoader.h>
#include "StringConverter.hpp"

Texture::Texture()
{
}

Texture::~Texture()
{
}

bool Texture::Initialize(ID3D11Device * device, const char* fileName)
{
	std::wstring wide = StringConverter::ToWideString("../Textures/" + std::string(fileName));

	if (FAILED(DirectX::CreateWICTextureFromFile(device, wide.c_str(), nullptr, &m_shaderResourceView)))
	{
		return false;
	}
	return true;
}

ID3D11ShaderResourceView * Texture::GetShaderResourceView()
{
	return m_shaderResourceView;
}