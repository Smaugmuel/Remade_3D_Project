#ifndef GUI_MANAGER_HPP
#define GUI_MANAGER_HPP
#include "../Math/Vector2.hpp"
#include <vector>
#include <string>
#include <memory>

namespace DirectX
{
	class SpriteFont;
	class SpriteBatch;
}

class Direct3D;
class TextureManager;

enum class Fonts
{
	COURIER_32,
	COMIC_SANS_MS_16
};
enum class GUI_Flags
{
	IS_RENDERED
};

struct GUI_Object
{
	int id;
	Vector2i position;
	Vector2i origin;
	
	bool flags[static_cast<unsigned int>(GUI_Flags::IS_RENDERED) + 1];

protected:
	GUI_Object() {}
};
struct GUI_Text : public GUI_Object
{
	Fonts font = Fonts::COURIER_32;
	std::string text = "";

	GUI_Text() : GUI_Object() {}
};
struct GUI_Image : public GUI_Object
{
	int index = -1;

	GUI_Image() : GUI_Object() {}
};

class GUIManager final
{
public:
	GUIManager();
	~GUIManager();

	bool Initialize(Direct3D* d3d, TextureManager* textureManager);

	void Render();

	int CreateText(const std::string& text, Vector2i position = Vector2i(0, 0), Fonts font = Fonts::COURIER_32, Vector2i origin = Vector2i(0, 0));
	int CreateImage(const std::string& imageFileName, Vector2i position = Vector2i(0, 0), Vector2i origin = Vector2i(0, 0));

	void RemoveText(int id);
	void RemoveImage(int id);

	void SetTextPosition(int id, Vector2i position);
	void SetTextOrigin(int id, Vector2i origin);
	void SetTextFont(int id, Fonts font);
	void SetText(int id, const std::string& text);
	void SetTextFlag(int id, GUI_Flags flag, bool state);

	void SetImagePosition(int id, Vector2i position);
	void SetImageOrigin(int id, Vector2i origin);
	void SetImageName(int id, const std::string& name);
	void SetImageFlag(int id, GUI_Flags flag, bool state);

	Vector2i GetDimensionsOfText(const std::string& text, Fonts font = Fonts::COURIER_32) const;
	Vector2i GetDimensionsOfText(const std::wstring& text, Fonts font = Fonts::COURIER_32) const;

private:
	int GetTextIndex(int id) const;
	int GetImageIndex(int id) const;

	static const unsigned int NR_OF_FONTS = (unsigned int)Fonts::COMIC_SANS_MS_16 + 1;

	std::vector<GUI_Text> m_texts;
	std::vector<GUI_Image> m_images;

	std::unique_ptr<DirectX::SpriteFont> m_spriteFonts[NR_OF_FONTS];
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	Direct3D* m_d3d;
	TextureManager* m_textureManager;
};

#endif