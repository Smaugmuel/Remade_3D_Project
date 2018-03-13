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
		else if (std::fabsf(e) > obb.halfSides[i])
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

IntersectionData RayVsOBB(const Ray & ray, const OBB & obb)
{
	return RayVsOBB(ray.origin, ray.direction, obb);
}

IntersectionData RayVSSphere(const Vector3f & origin, const Vector3f & direction, const Sphere & sphere)
{
	// Vector from ray origin to sphere center
	Vector3f toCenter = sphere.center - origin;

	// Distance of toCenter projected on direction
	float s = toCenter.dot(direction);

	// Squared distance of toVector (for better performance)
	float distToCenterSquared = toCenter.lengthSquared();

	// Check if direction is away from sphere and origin is outside of sphere
	if (s < 0 && distToCenterSquared > sphere.radius2)
	{
		return IntersectionData();
	}

	// Squared distance of vector from closest point on ray to sphere center
	float m2 = distToCenterSquared - s * s;

	// Check if ray misses sphere
	if (m2 > sphere.radius2)
	{
		return IntersectionData();
	}

	// Distance along ray, from point closest to sphere center, to sphere hull
	float q = sqrtf(sphere.radius2 - m2);

	float t;

	// Determine which intersection point is the closest
	if (distToCenterSquared > sphere.radius2)
	{
		// Origin is outside sphere, pointing towards it
		t = s - q;
	}
	else
	{
		// Origin is inside sphere, pointing out from it
		t = s + q;
	}

	return IntersectionData(true, t);
}

IntersectionData RayVSSphere(const Ray & ray, const Sphere & sphere)
{
	return RayVSSphere(ray.origin, ray.direction, sphere);
}



Collision* Singleton<Collision>::s_instance = nullptr;

Collision::Collision()
{
}

Collision::~Collision()
{
}

bool Collision::FrustumVSOBB(const Frustum & frustum, const OBB & obb)
{
	Vector3f halfSides[3];
	Vector3f diagonals[4];

	for (unsigned int i = 0; i < 3; i++)
	{
		halfSides[i] = obb.vectors[i] * obb.halfSides[i];
	}

	diagonals[0] = halfSides[0] + halfSides[1] + halfSides[2];
	diagonals[1] = halfSides[0] + halfSides[1] - halfSides[2];
	diagonals[2] = halfSides[0] - halfSides[1] + halfSides[2];
	diagonals[3] = halfSides[0] - halfSides[1] - halfSides[2];

	for (unsigned int i = 0; i < 6; i++)
	{
		float highDot = 0.0f;
		unsigned int highIndex = 0;
		int highIsPositive = 1;

		for (unsigned int j = 0; j < 4; j++)
		{
			float tempDot = frustum.planes[i].normal.dot(diagonals[j]);
			int isPositive = 1;

			if (tempDot < 0.0f)
			{
				tempDot = -tempDot;
				isPositive = -1;
			}

			if (tempDot > highDot)
			{
				highIndex = j;
				highDot = tempDot;
				highIsPositive = isPositive;
			}
		}

		if (DistancePlaneToPoint(frustum.planes[i], obb.center + diagonals[highIndex] * static_cast<float>(highIsPositive)) > 0.0f)
			return false;
	}

	return true;
}

bool Collision::FrustumVSPoint(const Frustum & frustum, Vector3f point)
{
	return false;
}

float Collision::DistancePlaneToPoint(const Plane & plane, Vector3f point)
{
	return plane.normal.dot(point) + plane.d;
}