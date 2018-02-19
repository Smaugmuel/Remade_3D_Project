#ifndef COLLISION_HPP
#define COLLISION_HPP
#include "Vector3.hpp"
#include "AABB.hpp"
#include "OBB.hpp"

struct IntersectionData
{
	IntersectionData(bool intersect = false, float t = 0.0f)
		: intersection(intersect)
		, distance(t)
	{
	}

	bool intersection;
	float distance;
};

//IntersectionData RayVsAABB(const Vector3f& origin, const Vector3f& direction, const AABB& aabb);
IntersectionData RayVsOBB(const Vector3f& origin, const Vector3f& direction, const OBB& obb);

#endif