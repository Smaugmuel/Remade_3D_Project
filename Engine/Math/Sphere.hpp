#ifndef SPHERE_HPP
#define SPHERE_HPP
#include "Vector3.hpp"

struct Sphere
{
	Sphere();
	Sphere(Vector3f _c, float r);

	Vector3f center;
	float radius;
	float radius2;
};

#endif