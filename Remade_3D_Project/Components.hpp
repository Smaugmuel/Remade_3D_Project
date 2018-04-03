#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP
#include "Vector3.hpp"

struct Component
{
protected:
	Component() {}
public:
	virtual ~Component() {}
};

struct TransformComponent : public Component
{
	Vector3f position;
	Vector3f rotation;
	Vector3f scale = Vector3f(1, 1, 1);
};

struct PhysicComponent : public Component
{
	Vector3f velocity;
	Vector3f acceleration;

	Vector3f angularVelocity;
	Vector3f angularAcceleration;
};

#endif