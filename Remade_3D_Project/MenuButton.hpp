#ifndef MENU_BUTTON_HPP
#define MENU_BUTTON_HPP
#include <functional>
#include <string>
#include "AABA.hpp"

template<typename CallbackClassType, typename ... ParamTypes>
class MenuButton
{
public:
	typedef void (CallbackClassType::*CallbackFunction)(const std::wstring&, ParamTypes...);

	MenuButton(CallbackClassType* callbackClass, CallbackFunction function, const std::wstring& text, const Vector2i& center, const Vector2i& halfDimensions)
	{
		m_callbackClass = callbackClass;
		m_callbackFunction = function;
		m_text = text;
		m_aaba.center = center;
		m_aaba.halfSides = halfDimensions;
	}
	~MenuButton()
	{
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
	AABA m_aaba;
};

#endif