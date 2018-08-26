#ifndef TEXT_BUTTON_MANAGER_HPP
#define TEXT_BUTTON_MANAGER_HPP
#include "../Engine/Math/AABA.hpp"
#include "../Engine/Core/Engine.hpp"
#include <functional>
#include <string>
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
class TextMenuButton
{
public:
	typedef void (CallbackClassType::*CallbackFunction)(const std::wstring&, ParamTypes...);

	TextMenuButton()
	{
	}
	~TextMenuButton()
	{
	}

	void Initialize(CallbackClassType* callbackClass, CallbackFunction function, const std::wstring& text, const Vector2i& center, const Vector2i& halfDimensions)
	{
		m_guiManager = Engine::Get()->GetGUIManager();
		m_id = m_guiManager->CreateText(StringConverter::ToString(text).value(), center, Fonts::COURIER_32, halfDimensions);

		m_callbackClass = callbackClass;
		m_callbackFunction = function;
		m_text = text;
		m_aaba.center = center;
		m_aaba.halfSides = halfDimensions;
	}
	void RemoveText()
	{
		m_guiManager->RemoveText(m_id);
	}

	void EnableRendering()
	{
		m_guiManager->SetTextFlag(m_id, GUI_Flags::IS_RENDERED, true);
	}
	void DisableRendering()
	{
		m_guiManager->SetTextFlag(m_id, GUI_Flags::IS_RENDERED, false);
	}

	void ProcessMouseClick(Vector2i mousePos, const ParamTypes& ... args)
	{
		if (m_aaba.Contains(mousePos))
		{
			(m_callbackClass->*(m_callbackFunction))(m_text, args...);
		}
	}

	void SetPosition(const Vector2i& position)
	{
		m_aaba.center = position;
		m_guiManager->SetTextPosition(m_id, position);
	}
	const std::wstring& GetText() const
	{
		return m_text;
	}
	const AABA& GetAABA() const
	{
		return m_aaba;
	}

private:
	CallbackClassType * m_callbackClass;
	CallbackFunction m_callbackFunction;
	std::wstring m_text;
	int m_id;
	AABA m_aaba;
	GUIManager* m_guiManager;
};


template<typename CallbackClassType, typename ... ParamTypes>
class TextButtonManager
{
public:
	typedef void (CallbackClassType::*CallbackFunction)(const std::wstring&, ParamTypes...);

	TextButtonManager()
	{
	}
	~TextButtonManager()
	{
		for (unsigned int i = 0; i < m_buttons.size(); i++)
		{
			m_buttons[i].RemoveText();
		}
	}
	
	void CreateButton(CallbackClassType* callbackClass, CallbackFunction function, const std::wstring& text, const Vector2i& center, const Vector2i& halfDimensions)
	{
		m_buttons.push_back(TextMenuButton<CallbackClassType, ParamTypes...>());
		m_buttons.back().Initialize(
			callbackClass,
			function,
			text,
			center,
			halfDimensions
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

	void EnableRendering()
	{
		for (unsigned int i = 0; i < m_buttons.size(); i++)
		{
			m_buttons[i].EnableRendering();
		}
	}
	void DisableRendering()
	{
		for (unsigned int i = 0; i < m_buttons.size(); i++)
		{
			m_buttons[i].DisableRendering();
		}
	}

	void ProcessMouseClick(const Vector2i& mousePos, const ParamTypes& ... args)
	{
		for (unsigned int i = 0; i < m_buttons.size(); i++)
		{
			m_buttons[i].ProcessMouseClick(mousePos, args...);
		}
	}

	std::vector<TextMenuButton<CallbackClassType, ParamTypes...>>& RetrieveButtons()
	{
		return m_buttons;
	}

private:
	std::vector<TextMenuButton<CallbackClassType, ParamTypes...>> m_buttons;
};

#endif