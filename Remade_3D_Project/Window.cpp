#include "Window.hpp"

Window* Singleton<Window>::s_instance = nullptr;

Window::Window()
{
	
}

Window::~Window()
{
}

bool Window::Initialize(const Vector2i& dimensions)
{
	if (dimensions.x <= 0 || dimensions.y <= 0)
	{
		return false;
	}

	m_dimensions = dimensions;
	windowHandle = this;
	m_programName = L"Remade_Project";
	m_hInstance = GetModuleHandle(NULL);

	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = m_hInstance;
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszClassName = m_programName;

	RegisterClassEx(&wcex);

	int x = (GetSystemMetrics(SM_CXSCREEN) - dimensions.x) / 2;
	int y = (GetSystemMetrics(SM_CYSCREEN) - dimensions.y) / 2;

	RECT rc = { x, y, x + dimensions.x, y + dimensions.y };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	m_windowHandle = CreateWindow(
		m_programName,
		m_programName,
		WS_OVERLAPPEDWINDOW,
		rc.left,
		rc.top,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		m_hInstance,
		nullptr);

	ShowWindow(m_windowHandle, SW_SHOW);
	SetForegroundWindow(m_windowHandle);
	SetFocus(m_windowHandle);

	return true;
}

const HWND& Window::GetHandle() const
{
	return m_windowHandle;
}
const Vector2i& Window::GetDimensions() const
{
	return m_dimensions;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}