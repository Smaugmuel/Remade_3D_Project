#ifndef AABB_HPP
#define AABB_HPP
#include "Vector3.hpp"

struct AABB
{
	AABB()
	{
	}

	bool Intersects(const AABB& other) const
	{
		if (center.x - halfSides.x > other.center.x + other.halfSides.x)
			return false;
		if (center.x + halfSides.x < other.center.x - other.halfSides.x)
			return false;
		if (center.y - halfSides.y > other.center.y + other.halfSides.y)
			return false;
		if (center.y + halfSides.y < other.center.y - other.halfSides.y)
			return false;
		if (center.z - halfSides.z > other.center.z + other.halfSides.z)
			return false;
		if (center.z + halfSides.z < other.center.z - other.halfSides.z)
			return false;
		return true;
	}
	bool Contains(const AABB& other) const
	{
		if (center.x - halfSides.x > other.center.x - other.halfSides.x)
			return false;
		if (center.x + halfSides.x < other.center.x + other.halfSides.x)
			return false;
		if (center.y - halfSides.y > other.center.y - other.halfSides.y)
			return false;
		if (center.y + halfSides.y < other.center.y + other.halfSides.y)
			return false;
		if (center.z - halfSides.z > other.center.z - other.halfSides.z)
			return false;
		if (center.z + halfSides.z < other.center.z + other.halfSides.z)
			return false;
		return true;
	}

	Vector3f center;
	Vector3f halfSides;
};

#endif