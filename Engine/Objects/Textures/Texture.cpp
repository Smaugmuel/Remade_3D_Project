#include "Texture.hpp"
#include "../../../packages/directxtk_desktop_2015.2018.7.3.1/include/WICTextureLoader.h"
#include "../../Misc/StringConverter.hpp"

Texture::Texture()
{
}

Texture::~Texture()
{
}

bool Texture::Initialize(ID3D11Device * device, const char* fileName)
{
	std::optional<std::wstring> wstr = StringConverter::ToWideString("../Textures/" + std::string(fileName));
	if (!wstr)
	{
		return false;
	}

	if (FAILED(DirectX::CreateWICTextureFromFile(device, wstr.value().c_str(), nullptr, &m_shaderResourceView)))
	{
		return false;
	}
	return true;
}

ID3D11ShaderResourceView * Texture::GetShaderResourceView()
{
	return m_shaderResourceView;
}