#ifndef INPUT_HPP
#define INPUT_HPP
#include "Vector2.hpp"
#include "Singleton.hpp"

class Input final : public Singleton<Input>
{
	friend class Singleton<Input>;

	Input();
	~Input();

public:
	void Update();

	bool IsKeyDown(unsigned int key) const;
	bool IsKeyPressed(unsigned int key) const;
	const Vector2i& MousePosition() const;
	const Vector2i MouseMovement() const;

private:
	bool m_keysDown[256];
	bool m_keysDownPrev[256];

	Vector2i m_mousePos;
	Vector2i m_mousePosPrev;
};

#endif