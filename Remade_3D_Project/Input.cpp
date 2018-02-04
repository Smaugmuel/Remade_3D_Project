#include "Input.hpp"
#include "Window.hpp"

Input* Singleton<Input>::s_instance = nullptr;

Input::Input()
{
	for (unsigned int i = 0; i < 256; i++)
	{
		m_keysDown[i] = false;
		m_keysDownPrev[i] = false;
	}
}

Input::~Input()
{
}

void Input::Update()
{
	RECT rect = { NULL };
	POINT windowPos;
	POINT cursorPos;

	for (unsigned int i = 0; i < 256; i++)
	{
		m_keysDownPrev[i] = m_keysDown[i];
		m_keysDown[i] = static_cast<bool>(GetAsyncKeyState(i) & 0x8000);
	}

	// Upper left corner of window
	GetWindowRect(Window::Get()->GetHandle(), &rect);
	windowPos.x = rect.left;
	windowPos.y = rect.top;

	// Position of cursor relative to screen
	GetCursorPos(&cursorPos);

	m_mousePosPrev = m_mousePos;
	// Position of cursor relative to window
	m_mousePos.x = (float)(cursorPos.x - windowPos.x);
	m_mousePos.y = (float)(cursorPos.y - windowPos.y);
}

bool Input::IsKeyDown(unsigned int key) const
{
	return m_keysDown[key];
}

bool Input::IsKeyPressed(unsigned int key) const
{
	return m_keysDown[key] && !m_keysDownPrev[key];
}

const Vector2f& Input::MousePosition() const
{
	return m_mousePos;
}

const Vector2f Input::MouseMovement() const
{
	return m_mousePos - m_mousePosPrev;
}