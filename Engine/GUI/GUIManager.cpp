#include "GUIManager.hpp"
#include "../FrameWork/Direct3D.hpp"
#include "../Misc/StringConverter.hpp"
//#include "../Objects/Textures/TextureStorage.hpp"
#include "../Objects/Textures/TextureStorageV2.hpp"

#include "../../packages/directxtk_desktop_2015.2018.7.3.1/include/SpriteFont.h"
#include "../../packages/directxtk_desktop_2015.2018.7.3.1/include/SpriteBatch.h"

GUIManager* Singleton<GUIManager>::s_instance = nullptr;

GUIManager::GUIManager() : m_spriteBatch(nullptr)
{
	Direct3D* d3d = Direct3D::Get();

	for (unsigned int i = 0; i < NR_OF_FONTS; i++)
	{
		m_spriteFonts[i] = nullptr;
	}

	m_spriteFonts[static_cast<unsigned int>(Fonts::COURIER_32)] =
		std::make_unique<DirectX::SpriteFont>(d3d->GetDevice(), L"../Fonts/courier32.spritefont");
	m_spriteFonts[static_cast<unsigned int>(Fonts::COMIC_SANS_MS_16)] =
		std::make_unique<DirectX::SpriteFont>(d3d->GetDevice(), L"../Fonts/comicSansMS16.spritefont");

	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(d3d->GetDeviceContext());
}
GUIManager::~GUIManager()
{
	for (unsigned int i = 0; i < NR_OF_FONTS; i++)
	{
		m_spriteFonts[i].reset();
	}
	m_spriteBatch.reset();
}

void GUIManager::Render()
{
	Direct3D* d3d = Direct3D::Get();
	TextureStorageV2* textureStorage = TextureStorageV2::Get();
	unsigned int nTexts = m_texts.size();
	unsigned int nImages = m_images.size();

	// Prepare rendering
	d3d->DisableZBuffer();
	m_spriteBatch->Begin();

	// Render images
	for (unsigned int i = 0; i < nImages; i++)
	{
		GUI_Image* image = &m_images[i];

		if (!image->flags[static_cast<unsigned int>(GUI_Flags::IS_RENDERED)])
		{
			continue;
		}
		m_spriteBatch->Draw(
			textureStorage->GetTexture(image->index),
			DirectX::XMFLOAT2(static_cast<float>(image->position.x), static_cast<float>(image->position.y)),
			DirectX::Colors::White
		);
	}

	// Render texts
	for (unsigned int i = 0; i < nTexts; i++)
	{
		GUI_Text* text = &m_texts[i];

		if (!text->flags[static_cast<unsigned int>(GUI_Flags::IS_RENDERED)])
		{
			continue;
		}
		m_spriteFonts[static_cast<unsigned int>(text->font)]->DrawString(
			m_spriteBatch.get(),
			StringConverter::ToWideString(text->text).value().c_str(),
			DirectX::XMFLOAT2(static_cast<float>(text->position.x), static_cast<float>(text->position.y)),
			DirectX::Colors::White,
			0.0f,
			DirectX::XMFLOAT2(static_cast<float>(text->origin.x), static_cast<float>(text->origin.y))
		);
	}

	// Finish rendering
	m_spriteBatch->End();
	d3d->EnableZBuffer();
	d3d->SetDefaultBlendState();
}

int GUIManager::CreateText(const std::string & text, Vector2i position, Fonts font, Vector2i origin)
{
	static int id = -1;
	id++;

	GUI_Text gui_text;
	gui_text.id = id;
	gui_text.position = position;
	gui_text.origin = origin;
	gui_text.font = font;
	gui_text.text = text;
	gui_text.flags[static_cast<unsigned int>(GUI_Flags::IS_RENDERED)] = true;

	m_texts.push_back(gui_text);

	return id;
}
int GUIManager::CreateImage(const std::string & imageFileName, Vector2i position, Vector2i origin)
{
	static int id = -1;
	id++;

	GUI_Image gui_image;
	gui_image.id = id;
	gui_image.position = position;
	gui_image.origin = origin;
	gui_image.index = TextureStorageV2::Get()->GetIndexByName(imageFileName);
	gui_image.flags[static_cast<unsigned int>(GUI_Flags::IS_RENDERED)] = true;

	m_images.push_back(gui_image);

	return id;
}

void GUIManager::RemoveText(int id)
{
	switch (int index = GetTextIndex(id))
	{
	case -1:
		break;
	default:
		m_texts.erase(m_texts.begin() + index);
		break;
	}
}
void GUIManager::RemoveImage(int id)
{
	switch (int index = GetImageIndex(id))
	{
	case -1:
		break;
	default:
		m_images.erase(m_images.begin() + index);
		break;
	}
}

void GUIManager::SetTextPosition(int id, Vector2i position)
{
	switch (int index = GetTextIndex(id))
	{
	case -1:
		break;
	default:
		m_texts[index].position = position;
		break;
	}
}
void GUIManager::SetTextOrigin(int id, Vector2i origin)
{
	switch (int index = GetTextIndex(id))
	{
	case -1:
		break;
	default:
		m_texts[index].origin = origin;
		break;
	}
}
void GUIManager::SetTextFont(int id, Fonts font)
{
	switch (int index = GetTextIndex(id))
	{
	case -1:
		break;
	default:
		m_texts[index].font = font;
		break;
	}
}
void GUIManager::SetText(int id, const std::string & text)
{
	switch (int index = GetTextIndex(id))
	{
	case -1:
		break;
	default:
		m_texts[index].text = text;
		break;
	}
}
void GUIManager::SetTextFlag(int id, GUI_Flags flag, bool state)
{
	switch (int index = GetTextIndex(id))
	{
	case -1:
		break;
	default:
		m_texts[index].flags[static_cast<unsigned int>(flag)] = state;
		break;
	}
}

void GUIManager::SetImagePosition(int id, Vector2i position)
{
	switch (int index = GetImageIndex(id))
	{
	case -1:
		break;
	default:
		m_images[index].position = position;
		break;
	}
}
void GUIManager::SetImageOrigin(int id, Vector2i origin)
{
	switch (int index = GetImageIndex(id))
	{
	case -1:
		break;
	default:
		m_images[index].origin = origin;
		break;
	}
}
void GUIManager::SetImageName(int id, const std::string & name)
{
	switch (int index = GetImageIndex(id))
	{
	case -1:
		break;
	default:
		m_images[index].index = TextureStorageV2::Get()->GetIndexByName(name);
		break;
	}
}
void GUIManager::SetImageFlag(int id, GUI_Flags flag, bool state)
{
	switch (int index = GetImageIndex(id))
	{
	case -1:
		break;
	default:
		m_images[index].flags[static_cast<unsigned int>(flag)] = state;
		break;
	}
}

Vector2i GUIManager::GetDimensionsOfText(const std::string & text, Fonts font) const
{
	return GetDimensionsOfText(StringConverter::ToWideString(text).value());
}
Vector2i GUIManager::GetDimensionsOfText(const std::wstring & text, Fonts font) const
{
	tagRECT rect = m_spriteFonts[static_cast<unsigned int>(font)]->MeasureDrawBounds(text.c_str(), DirectX::XMFLOAT2(0, 0));

	return Vector2i(static_cast<int>(rect.right - rect.left), static_cast<int>(rect.bottom - rect.top));
}

int GUIManager::GetTextIndex(int id) const
{
	unsigned int n = m_texts.size();
	for (unsigned int i = 0; i < n; i++)
	{
		if (m_texts[i].id == id)
		{
			return i;
		}
	}
	return -1;
}
int GUIManager::GetImageIndex(int id) const
{
	unsigned int n = m_images.size();
	for (unsigned int i = 0; i < n; i++)
	{
		if (m_images[i].id == id)
		{
			return i;
		}
	}
	return -1;
}