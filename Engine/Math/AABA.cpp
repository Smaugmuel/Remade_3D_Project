#include "AABA.hpp"

AABA::AABA()
{
}

bool AABA::Intersects(const AABA & other) const
{
	if (center.x - halfSides.x > other.center.x + other.halfSides.x)
		return false;
	if (center.x + halfSides.x < other.center.x - other.halfSides.x)
		return false;
	if (center.y - halfSides.y > other.center.y + other.halfSides.y)
		return false;
	if (center.y + halfSides.y < other.center.y - other.halfSides.y)
		return false;
	return true;
}

bool AABA::Contains(const AABA & other) const
{
	if (center.x - halfSides.x >= other.center.x - other.halfSides.x)
		return false;
	if (center.x + halfSides.x <= other.center.x + other.halfSides.x)
		return false;
	if (center.y - halfSides.y >= other.center.y - other.halfSides.y)
		return false;
	if (center.y + halfSides.y <= other.center.y + other.halfSides.y)
		return false;
	return true;
}

bool AABA::Contains(const Vector2f & point) const
{
	if (point.x < center.x - halfSides.x)
		return false;
	if (point.x > center.x + halfSides.x)
		return false;
	if (point.y < center.y - halfSides.y)
		return false;
	if (point.y > center.y + halfSides.y)
		return false;

	return true;
}