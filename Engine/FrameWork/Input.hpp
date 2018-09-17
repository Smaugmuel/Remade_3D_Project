#ifndef INPUT_HPP
#define INPUT_HPP
#include "../Math/Vector2.hpp"
#include <Windows.h>		// Provides defined keys

class Input final
{
public:
	Input();
	~Input();

	bool Initialize(HWND__* handle);

	void Update();

	bool IsKeyDown(unsigned int key) const;
	bool IsKeyPressed(unsigned int key) const;
	const Vector2i& MousePosition() const;
	Vector2i MouseMovement() const;

private:
	//friend class Singleton<Input>;

	bool m_keysDown[256];
	bool m_keysDownPrev[256];

	Vector2i m_mousePos;
	Vector2i m_mousePosPrev;

	HWND__* m_hwnd;
};

#endif