#ifndef BUTTON_MANAGER_HPP
#define BUTTON_MANAGER_HPP
#include "MenuButton.hpp"
#include <vector>

/* Notes */
// =========================================================================================
// | The callback class type defines the class where the callback functions are located    |
// | One manager can only call functions within that class                                 |
// ===================================================================================================
// | The syntax of this kind of function is "void FunctionName(ParamTypes...)"                       |
// | This means that the function needs to return nothing, but can receive any parameters            |
// ---------------------------------------------------------------------------------------------------
// | I.e. void PressedPlay(Vector2i, float, unsigned int)                                            |
// ---------------------------------------------------------------------------------------------------
// | These data types need to be sent when calling ProcessMouseClick(), not when creating the button |
// | Most cases probably need no parameters                                                          |
// ============================================================================================================
// | When adding a button, use the following syntax                                                           |
// ------------------------------------------------------------------------------------------------------------
// | I.e. mainMenuButtonManager->CreateButton(this, &MainMenuState::PressedPlay, "PLAY", 100, 100, 100, 100); |
// ===============================================================================================================
// | The return type of the function pointer could also be set as a template, and it's easy to add if one wishes |
// | However, the usage of the ProcessMouseClick() in a generic case becomes either difficult or very narrow     |
// | For instance, how would even the simple case of a bool as a return type behave as a GENERIC case?           |
// ---------------------------------------------------------------------------------------------------------------
// | for (unsigned int i = 0; i < m_buttons.size(); i++)                                                         |
// | {                                                                                                           |
// |     if (m_buttons[i].ProcessMouseClick(mousePos, args...) == ReturnType(??????))                            |
// |     {                                                                                                       |
// |         return ReturnType(??????);                                                                          |
// |     }                                                                                                       |
// | }                                                                                                           |
// | return ReturnType(??????);                                                                                  |
// ===============================================================================================================


template<typename CallbackClassType, typename ... ParamTypes>
class ButtonManager
{
public:
	typedef void (CallbackClassType::*CallbackFunction)(const std::wstring&, ParamTypes...);

	ButtonManager()
	{
	}
	~ButtonManager()
	{
	}
	
	void CreateButton(CallbackClassType* callbackClass, CallbackFunction function, const std::wstring& text, const Vector2i& center, const Vector2i& halfDimensions)
	{
		m_buttons.push_back(
			MenuButton<CallbackClassType, ParamTypes...>(
				callbackClass,
				function,
				text,
				center,
				halfDimensions
			)
		);
	}
	void CreateButton(CallbackClassType* callbackClass, CallbackFunction function, const std::wstring& text, int centerX, int centerY, int halfWidth, int halfHeight)
	{
		CreateButton(callbackClass, function, text, Vector2i(centerX, centerY), Vector2i(halfWidth, halfHeight));
	}

	void ClearButtons()
	{
		m_buttons.clear();
	}

	void ProcessMouseClick(const Vector2i& mousePos, const ParamTypes& ... args)
	{
		for (unsigned int i = 0; i < m_buttons.size(); i++)
		{
			m_buttons[i].ProcessMouseClick(mousePos, args...);
		}
	}

	std::vector<MenuButton<CallbackClassType, ParamTypes...>>& RetrieveButtons()
	{
		return m_buttons;
	}

private:
	std::vector<MenuButton<CallbackClassType, ParamTypes...>> m_buttons;
};

#endif