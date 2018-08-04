#include "AABB.hpp"

AABB::AABB()
{
}

bool AABB::Intersects(const AABB & other) const
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

bool AABB::Contains(const AABB & other) const
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

bool AABB::Contains(const Vector3f & point) const
{
	if (point.x < center.x - halfSides.x)
		return false;
	if (point.x > center.x + halfSides.x)
		return false;
	if (point.y < center.y - halfSides.y)
		return false;
	if (point.y > center.y + halfSides.y)
		return false;
	if (point.z < center.z - halfSides.z)
		return false;
	if (point.z > center.z + halfSides.z)
		return false;

	return true;
}