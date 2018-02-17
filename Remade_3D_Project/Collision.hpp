#ifndef COLLISION_HPP
#define COLLISION_HPP
#include "Vector3.hpp"
#include "AABB.hpp"

struct IntersectionData
{
	bool intersection;
	float distance;
};

//IntersectionData RayVsAABB(const Vector3f& origin, const Vector3f& direction, const AABB& aabb);

#endif