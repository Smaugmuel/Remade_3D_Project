#ifndef SPHERE_HPP
#define SPHERE_HPP
#include "Vector3.hpp"

struct Sphere
{
	Sphere()
	{
		center = Vector3f(0, 0, 0);
		radius = 1.0f;
		radius2 = 1.0f;
	}
	Sphere(Vector3f c, float r)
	{
		center = c;
		radius = r;
		radius2 = radius * radius;
	}

	Vector3f center;
	float radius;
	float radius2;
};

#endif