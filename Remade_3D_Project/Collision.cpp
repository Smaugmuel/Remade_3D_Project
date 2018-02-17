#include "Collision.hpp"
#include <algorithm>

//IntersectionData RayVsAABB(const Vector3f & origin, const Vector3f & direction, const AABB & aabb)
//{
//	Vector3f planePointX = aabb.center;
//	Vector3f planePointY = aabb.center;
//	Vector3f planePointZ = aabb.center;
//
//	if (direction.x < 0)
//		planePointX.x += aabb.halfSides.x;
//	else
//		planePointX.x -= aabb.halfSides.x;
//
//	if (direction.y < 0)
//		planePointY.y += aabb.halfSides.y;
//	else
//		planePointY.y -= aabb.halfSides.y;
//
//	if (direction.z < 0)
//		planePointZ.z += aabb.halfSides.z;
//	else
//		planePointZ.z -= aabb.halfSides.z;
//
//	float tX = Vector3f(1, 0, 0).dot(planePointX - origin) / Vector3f(1, 0, 0).dot(direction);
//	float tY = Vector3f(0, 1, 0).dot(planePointY - origin) / Vector3f(0, 1, 0).dot(direction);
//	float tZ = Vector3f(0, 0, 1).dot(planePointZ - origin) / Vector3f(0, 0, 1).dot(direction);
//
//	float t = std::max(std::max(tX, tY), tZ);
//
//	if ()
//	{
//
//	}
//
//	return false;
//}