#ifndef AABB_HPP
#define AABB_HPP
#include "Vector3.hpp"

struct AABB
{
	AABB();

	bool Intersects(const AABB& other) const;
	bool Contains(const AABB& other) const;
	bool Contains(const Vector3f& point) const;

	Vector3f center;
	Vector3f halfSides;
};

#endif