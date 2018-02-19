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
//	// Find at what distance ray intersects each plane
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

IntersectionData RayVsOBB(const Vector3f & origin, const Vector3f & direction, const OBB & obb)
{
	float t_min = -INFINITY;
	float t_max = INFINITY;

	Vector3f toCenter = obb.center - origin;

	for (unsigned int i = 0; i < 3; i++)
	{
		// Distance from origin to center projected on obb face normal
		float e = obb.vectors[i].dot(toCenter);

		// Cosine angle between direction and obb face normal
		float f = obb.vectors[i].dot(direction);

		// Check if direction is perpendicular with face normal
		// In other words, if it is parallell to the face
		if (std::fabsf(f) > 0.000001f)
		{
			// Distances, along obb's face normal, from origin to its edges
			float near = (e + obb.halfSides[i]);
			float far = (e - obb.halfSides[i]);

			// Optimization
			float div_f = 1.0f / f;

			// Find actual length of vector from origin to intersections
			// This uses trigonometry: cos(v) = adjacent / hypotenuse
			// Hypothenuse = adjacent / cos(v)
			near *= div_f;
			far *= div_f;

			// Ensure values are correct
			if (far < near)
			{
				float temp = far;
				far = near;
				near = temp;
			}

			// Update closest intersection to as far as possible
			if (near > t_min)
			{
				t_min = near;
			}

			// Update furthest intersection to as near as possible
			if (far < t_max)
			{
				t_max = far;
			}

			// Now, t_min and t_max might not be distances to faces in parallell planes
			// Because a ray can intersect an obb in two non-parallell faces

			// Check if ray missed or went past obb
			if (t_min > t_max)
			{
				return IntersectionData();
			}

			// Check if ray origin is in front of intersection
			if (t_max < 0)
			{
				return IntersectionData();
			}
		}

		// Check if ray would go past obb
		else if (std::fabsf(e) < obb.halfSides[i])
		{
			return IntersectionData();
		}
	}

	// Check if origin is not inside obb
	if (t_min > 0)
	{
		return IntersectionData(true, t_min);
	}

	return IntersectionData(true, t_max);
}
