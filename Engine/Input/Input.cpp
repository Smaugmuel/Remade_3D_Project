#include "Input.hpp"
#include "../FrameWork/Window.hpp"

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
	for (unsigned int i = 0; i < 256; i++)
	{
		m_keysDownPrev[i] = m_keysDown[i];
		m_keysDown[i] = static_cast<bool>(GetAsyncKeyState(i) & 0x8000);

		if (m_keysDown[i])
		{
			int apa = 0;
		}
	}

	// Coordinates on screen
	tagPOINT mouseCoords;
	GetCursorPos(&mouseCoords);

	// Coordinates on window, below header
	ScreenToClient(Window::Get()->GetHandle(), &mouseCoords);

	m_mousePosPrev = m_mousePos;
	m_mousePos.x = mouseCoords.x;
	m_mousePos.y = mouseCoords.y;
}

bool Input::IsKeyDown(unsigned int key) const
{
	return m_keysDown[key];
}

bool Input::IsKeyPressed(unsigned int key) const
{
	return m_keysDown[key] && !m_keysDownPrev[key];
}

const Vector2i& Input::MousePosition() const
{
	return m_mousePos;
}

const Vector2i Input::MouseMovement() const
{
	return m_mousePos - m_mousePosPrev;
}