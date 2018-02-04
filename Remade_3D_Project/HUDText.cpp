#include "HUDText.hpp"
#include <d3d11.h>
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"

// Converting strings
#include <locale>
#include <codecvt>
#include <string>

#include "Direct3D.hpp"

HUDText::HUDText()
{
}

HUDText::~HUDText()
{
}

bool HUDText::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext)
{
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"../Fonts/myfile.spritefont");
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(deviceContext);
	m_position = Vector2f(0.0f, 0.0f);
	m_origin = Vector2f(0.0f, 0.0f);

	m_text = "";

	return true;
}

void HUDText::SetText(std::string text)
{
	m_text = text;
}

void HUDText::SetPosition(Vector2f position)
{
	m_position = position;
}

void HUDText::SetOrigin(Vector2f origin)
{
	m_origin = origin;
}

void HUDText::CenterOrigin()
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring output = converter.from_bytes(m_text);
	
	m_origin = DirectX::SimpleMath::Vector2(DirectX::SimpleMath::Vector2(m_spriteFont->MeasureString(output.c_str())) / 2.0f);
}

void HUDText::Render(ID3D11DeviceContext * deviceContext)
{
	// Prepare for rendering
	Direct3D::Get()->DisableZBuffer();

	// Convert from string to wstring
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring output = converter.from_bytes(m_text);	

	// Convert to SimpleMath Vector
	DirectX::SimpleMath::Vector2 origin(m_origin.x, m_origin.y);
	DirectX::SimpleMath::Vector2 position(m_position.x, m_position.y);

	// Render
	m_spriteBatch->Begin();
	m_spriteFont->DrawString(m_spriteBatch.get(), output.c_str(), position, DirectX::Colors::White, 0.0f, origin);
	m_spriteBatch->End();

	// Reset to normal states
	Direct3D::Get()->SetDefaultBlendState();
	Direct3D::Get()->EnableZBuffer();
}
