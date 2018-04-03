#include "HUDText.hpp"
#include <d3d11.h>
#include "SimpleMath.h"
#include "SpriteFont.h"
#include "SpriteBatch.h"
#include "StringConverter.hpp"

HUDText::HUDText()
{
}

HUDText::~HUDText()
{
	m_font.reset();
	m_spriteBatch.reset();
}

void HUDText::CreateSpriteBatch(ID3D11DeviceContext* deviceContext)
{
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(deviceContext);
}

void HUDText::LoadFont(ID3D11Device * device, const char * fileName)
{
	std::wstring fileNameWide = StringConverter::ToWideString(fileName);

	m_font = std::make_unique<DirectX::SpriteFont>(device, fileNameWide.c_str());
}

void HUDText::Render()
{
	m_spriteBatch->Begin();

	DirectX::SimpleMath::Vector2 origin = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
	DirectX::SimpleMath::Vector2 position(m_fontPos.x, m_fontPos.y);

	m_font->DrawString(m_spriteBatch.get(), m_text.c_str(), position, DirectX::Colors::White, 0.0f, origin);

	m_spriteBatch->End();
}

void HUDText::SetText(const char * text)
{
	m_text = StringConverter::ToWideString(text);
}

void HUDText::SetPosition(Vector2f position)
{
	m_fontPos.x = position.x;
	m_fontPos.y = position.y;
}