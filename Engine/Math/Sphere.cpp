#include "Sphere.hpp"

Sphere::Sphere()
{
	center = Vector3f(0, 0, 0);
	radius = 1.0f;
	radius2 = 1.0f;
}

Sphere::Sphere(Vector3f _c, float r)
{
	center = _c;
	radius = r;
	radius2 = radius * radius;
}