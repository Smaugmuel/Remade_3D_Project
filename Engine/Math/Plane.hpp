#ifndef PLANE_HPP
#define PLANE_HPP
#include "Vector3.hpp"

struct Plane
{
	Plane();

	Vector3f normal;
	float d;
};

#endif