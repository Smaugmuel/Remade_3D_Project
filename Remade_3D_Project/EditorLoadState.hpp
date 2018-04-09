#ifndef EDITOR_LOAD_STATE_HPP
#define EDITOR_LOAD_STATE_HPP
#include "EditorState.hpp"
#include <string>

template<typename CallbackClassType, typename ... ParamTypes> class Menu;

namespace DirectX
{
	class SpriteFont;
	class SpriteBatch;
}

class EditorLoadState : public EditorState
{
public:
	EditorLoadState();
	~EditorLoadState();

	bool Initialize() override;
	void ProcessInput() override;
	void Update(float dt) override;
	void Render() override;
	void RenderHUD() override;

private:
	void CreateButtonsFromScenesInFolder();
	void LoadScene(const std::wstring& sceneName);

	Menu<EditorLoadState>* m_sceneMenu;

	DirectX::SpriteFont* m_spriteFont;
	DirectX::SpriteBatch* m_spriteBatch;
};

#endif