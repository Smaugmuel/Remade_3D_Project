#include "TextureStorageV2.hpp"
#include "../../../packages/directxtk_desktop_2015.2018.7.3.1/include/WICTextureLoader.h"
#include "../../Misc/StringConverter.hpp"
//#include "../../FrameWork/Direct3D.hpp"
//#include "../../Core/Engine.hpp"
#include "../../FrameWork/FrameWork.hpp"

//TextureStorageV2* Singleton<TextureStorageV2>::s_instance = nullptr;

TextureStorageV2::TextureStorageV2()
{
}

TextureStorageV2::~TextureStorageV2()
{
}

bool TextureStorageV2::Initialize(FrameWork * frameWork)
{
	m_frameWork = frameWork;
	if (LoadTexture("Missing.jpg") == -1)
		return false;
	return true;
}

int TextureStorageV2::LoadTexture(const std::string & name)
{
	if (int index = GetIndexByName(name); index != -1)
	{
		return index;
	}

	std::optional<std::wstring> wstr = StringConverter::ToWideString("../Textures/" + std::string(name));
	if (!wstr)
	{
		return -1;
	}

	ID3D11ShaderResourceView* resource = nullptr;

	if (FAILED(DirectX::CreateWICTextureFromFile(m_frameWork->GetDirect3D()->GetDevice(), wstr.value().c_str(), nullptr, &resource)))
	//if (FAILED(DirectX::CreateWICTextureFromFile(Direct3D::Get()->GetDevice(), wstr.value().c_str(), nullptr, &resource)))
	{
		return -1;
	}

	m_textures.push_back(Texture(name, resource));
	
	return m_textures.size() - 1;
}

int TextureStorageV2::GetIndexByName(const std::string & name)
{
	unsigned int n = m_textures.size();
	for (unsigned int i = 0; i < n; i++)
	{
		if (m_textures[i].name == name)
		{
			return i;
		}
	}

	return -1;
}
const std::string & TextureStorageV2::GetNameByIndex(int index)
{
	return index >= 0 && static_cast<unsigned int>(index) < m_textures.size() ? m_textures[index].name : "";
}
ID3D11ShaderResourceView * TextureStorageV2::GetTexture(int index)
{
	return index >= 0 && static_cast<unsigned int>(index) < m_textures.size() ? m_textures[index].texture : nullptr;
}
//ID3D11ShaderResourceView * TextureStorageV2::GetTextureByName(const std::string & name)
//{
//	return GetTextureByIndex(GetIndexByName(name));
//}

unsigned int TextureStorageV2::GetNrOfTextures() const
{
	return m_textures.size();
}
