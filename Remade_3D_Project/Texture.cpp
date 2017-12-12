#include "Texture.hpp"
#include <WICTextureLoader.h>
#include <locale>
#include <codecvt>
#include <string>

Texture::Texture()
{
}

Texture::~Texture()
{
}

bool Texture::Initialize(ID3D11Device * device, const char* fileName)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	//std::string narrow = converter.to_bytes(wide_utf16_source_string);
	//std::wstring wide = converter.from_bytes(narrow_utf8_source_string);
	std::wstring wide = converter.from_bytes(fileName);

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