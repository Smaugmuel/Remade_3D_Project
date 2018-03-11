#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "Vector3.hpp"

class Character
{
public:
	Character();
	~Character();

	void Update(float dt);

	void SetPosition(Vector3f position);
	void SetVelocity(Vector3f velocity);
	void SetAcceleration(Vector3f acceleration);

	void SetLookDirection(Vector3f direction);

	void SetMovementSpeed(float speed);
	void ToggleMovementBoost();

	void SetIsMovingRight(bool b);
	void SetIsMovingLeft(bool b);
	void SetIsMovingForward(bool b);
	void SetIsMovingBackward(bool b);
	void SetIsMovingUp(bool b);
	void SetIsMovingDown(bool b);

	void Move(Vector3f movement);
	void MoveRight(float amount);
	void MoveForward(float amount);
	void MoveUp(float amount);

	Vector3f GetPosition() const;
	Vector3f GetVelocity() const;
	Vector3f GetAcceleration() const;

	Vector3f GetRight() const;
	Vector3f GetForward() const;
	Vector3f GetLookDirection() const;

private:
	bool m_isMovingRight;
	bool m_isMovingLeft;
	bool m_isMovingForward;
	bool m_isMovingBackward;
	bool m_isMovingUp;
	bool m_isMovingDown;

	Vector3f m_position;
	Vector3f m_velocity;
	Vector3f m_acceleration;

	Vector3f m_lookDirection;

	float m_movementSpeed;
	float m_movementBoost;
};

#endif