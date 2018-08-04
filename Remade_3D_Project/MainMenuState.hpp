#ifndef MAIN_MENU_STATE_HPP
#define MAIN_MENU_STATE_HPP
#include "GameState.hpp"
#include <string>

template<typename CallbackClassType, typename ... ParamTypes> class TextButtonManager;

class MainMenuState final : public GameState
{
public:
	MainMenuState(StateMachineV2<GameState>* stateMachine);
	virtual ~MainMenuState();

	// Added with StateV2
	void OnEntry() final override;
	void OnExit() final override;

	bool Initialize() final override;
	void ProcessInput() final override;
	void Update(float dt) final override;
	void Render() final override;

private:
	void PressedPlay(const std::wstring& text);
	void PressedEdit(const std::wstring& text);
	void PressedExit(const std::wstring& text);

	TextButtonManager<MainMenuState> * m_buttonManager;
};

#endif