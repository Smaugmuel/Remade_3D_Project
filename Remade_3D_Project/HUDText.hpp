#ifndef HUD_TEXT_HPP
#define HUD_TEXT_HPP
#include <memory>
#include "Vector2.hpp"
#include <string>

namespace DirectX
{
	class SpriteFont;
	class SpriteBatch;

	namespace SimpleMath
	{
		struct Vector2;
	}
}
struct ID3D11Device;
struct ID3D11DeviceContext;

class HUDText
{
public:
	HUDText();
	~HUDText();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void Render(ID3D11DeviceContext* deviceContext);

	void SetText(std::string text);
	void SetPosition(Vector2f position);
	void SetOrigin(Vector2f origin);

	void CenterOrigin();

private:
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	Vector2f m_position;
	Vector2f m_origin;

	std::string m_text;
};

#endif