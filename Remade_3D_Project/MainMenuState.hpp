#ifndef MAIN_MENU_STATE_HPP
#define MAIN_MENU_STATE_HPP
#include "GameState.hpp"
#include <string>

namespace DirectX
{
	class SpriteFont;
	class SpriteBatch;
}

template<typename CallbackClassType, typename ... ParamTypes> class Menu;

class MainMenuState : public GameState
{
public:
	MainMenuState(StateMachine<GameState>* stateMachine);
	virtual ~MainMenuState();

	bool Initialize() override;
	void ProcessInput() override;
	void Update(float dt) override;
	void Render() override;

private:
	void PressedPlay(const std::wstring& text);
	void PressedEdit(const std::wstring& text);
	void PressedExit(const std::wstring& text);


	Menu<MainMenuState> * m_menu;

	DirectX::SpriteFont* m_spriteFont;
	DirectX::SpriteBatch* m_spriteBatch;
};

#endif