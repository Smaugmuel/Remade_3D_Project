#ifndef WINDOW_HPP
#define WINDOW_HPP
#include "Singleton.hpp"
#include <Windows.h>
#include "Vector2.hpp"

class Window final : public Singleton<Window>
{
	friend class Singleton<Window>;

	Window();
	~Window();

public:
	bool Initialize(const Vector2i& dimensions);

	const HWND& GetHandle() const;
	const Vector2i& GetDimensions() const;

private:
	LPCWSTR m_programName;
	HINSTANCE m_hInstance;
	HWND m_windowHandle;
	Vector2i m_dimensions;
};

static Window* windowHandle = nullptr;
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

#endif