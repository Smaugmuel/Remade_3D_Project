#ifndef HUD_TEXT_HPP
#define HUD_TEXT_HPP
#include <memory>
#include <string>
#include "../Math/Vector2.hpp"

namespace DirectX
{
	class SpriteFont;
	class SpriteBatch;
}

struct ID3D11Device;
struct ID3D11DeviceContext;

class HUDText
{
public:
	HUDText();
	~HUDText();

	void CreateSpriteBatch(ID3D11DeviceContext* deviceContext);
	void LoadFont(ID3D11Device* device, const char* fileName);

	void Render();

	void SetText(const char* text);
	void SetPosition(Vector2f position);

private:
	std::unique_ptr<DirectX::SpriteFont> m_font;
	Vector2f m_fontPos;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	std::wstring m_text;
};

#endif