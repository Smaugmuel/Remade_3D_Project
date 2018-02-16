#include "Character.hpp"

Character::Character()
{
	m_movementSpeed = 20.0f;

	m_isMovingRight = false;
	m_isMovingLeft = false;
	m_isMovingForward = false;
	m_isMovingBackward = false;
	m_isMovingUp = false;
	m_isMovingDown = false;
}

Character::~Character()
{
}

void Character::Update(float dt)
{
	float movementAmount = 20.0f * dt;
	Vector3f movements;
	movements += GetRight() * static_cast<float>(m_isMovingRight - m_isMovingLeft);
	movements += GetForward() * static_cast<float>(m_isMovingForward - m_isMovingBackward);
	movements += Vector3f(0, 1, 0) * static_cast<float>(m_isMovingUp - m_isMovingDown);

	movements.normalize();
	movements *= movementAmount;

	m_position += movements;
}

void Character::SetPosition(Vector3f position)
{
	m_position = position;
}

void Character::SetVelocity(Vector3f velocity)
{
	m_velocity = velocity;
}

void Character::SetAcceleration(Vector3f acceleration)
{
	m_acceleration = acceleration;
}

void Character::SetLookDirection(Vector3f direction)
{
	if (direction.x == 0.0f && direction.z == 0.0f)
		return;

	m_lookDirection = direction.normalized();
}

void Character::SetMovementSpeed(float speed)
{
	if (speed == 0.0f)
		return;
	m_movementSpeed = speed;
}

void Character::SetIsMovingRight(bool b)
{
	m_isMovingRight = b;
}

void Character::SetIsMovingLeft(bool b)
{
	m_isMovingLeft = b;
}

void Character::SetIsMovingForward(bool b)
{
	m_isMovingForward = b;
}

void Character::SetIsMovingBackward(bool b)
{
	m_isMovingBackward = b;
}

void Character::SetIsMovingUp(bool b)
{
	m_isMovingUp = b;
}

void Character::SetIsMovingDown(bool b)
{
	m_isMovingDown = b;
}

void Character::Move(Vector3f movement)
{
	m_position += movement;
}

void Character::MoveRight(float amount)
{
	Move(GetRight() * amount);
}

void Character::MoveForward(float amount)
{
	Move(GetForward() * amount);
}

void Character::MoveUp(float amount)
{
	Move(Vector3f(0, 1, 0) * amount);
}

Vector3f Character::GetPosition() const
{
	return m_position;
}

Vector3f Character::GetVelocity() const
{
	return m_velocity;
}

Vector3f Character::GetAcceleration() const
{
	return m_acceleration;
}

Vector3f Character::GetRight() const
{
	return GetForward().crossLH(Vector3f(0, 1, 0)).normalized();
}

Vector3f Character::GetForward() const
{
	Vector3f forward = GetLookDirection();
	forward.y = 0.0f;
	return forward.normalized();
}

Vector3f Character::GetLookDirection() const
{
	return m_lookDirection;
}
