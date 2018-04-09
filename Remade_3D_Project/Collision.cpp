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


bool Collision::OBBVSOBB(const OBB & b1, const OBB & b2)
{
	float squaredDistanceBetweenCenters = (b1.center - b2.center).lengthSquared();

	/* =================================== Early return 1: Inprecise bounding sphere intersection test =================================== */
	float radiusSquared1 = b1.halfSides[0] * b1.halfSides[0] + b1.halfSides[1] * b1.halfSides[1] + b1.halfSides[2] * b1.halfSides[2];
	float radiusSquared2 = b2.halfSides[0] * b2.halfSides[0] + b2.halfSides[1] * b2.halfSides[1] + b2.halfSides[2] * b2.halfSides[2];

	float sumOfSquaredRadii = radiusSquared1 + radiusSquared2;
	float squaredSumOfSquaredRadii = sumOfSquaredRadii * sumOfSquaredRadii;

	// Check if distance between centers is less than sum of squared radii
	if (squaredDistanceBetweenCenters > squaredSumOfSquaredRadii)
	{
		return false;
	}

	/* =================================== Early return 2: Bounding sphere intersection test ============================================= */
	float radius1 = std::sqrtf(radiusSquared1);
	float radius2 = std::sqrtf(radiusSquared2);
	
	float sumOfRadii = radius1 + radius2;
	float squaredSumOfRadii = sumOfRadii * sumOfRadii;

	// Check if distance between centers is less than sum of radii
	if (squaredDistanceBetweenCenters > squaredSumOfRadii)
	{
		return false;
	}

	/* =================================== Separating Axis Theorem intersection tests ==================================================== */
	Vector3f obbHalfEdges1[3];
	Vector3f obbHalfEdges2[3];

	Vector3f corners1[8];
	Vector3f corners2[8];

	// Calculate the vectors of both obb's half sides
	for (unsigned int i = 0; i < 3; i++)
	{
		obbHalfEdges1[i] = b1.vectors[i] * b1.halfSides[i];
		obbHalfEdges2[i] = b2.vectors[i] * b2.halfSides[i];
	}

	// Calculate corners of first OBB
	corners1[0] = b1.center - obbHalfEdges1[0] - obbHalfEdges1[1] - obbHalfEdges1[2];
	corners1[1] = b1.center - obbHalfEdges1[0] - obbHalfEdges1[1] + obbHalfEdges1[2];
	corners1[2] = b1.center - obbHalfEdges1[0] + obbHalfEdges1[1] - obbHalfEdges1[2];
	corners1[3] = b1.center - obbHalfEdges1[0] + obbHalfEdges1[1] + obbHalfEdges1[2];
	corners1[4] = b1.center + obbHalfEdges1[0] - obbHalfEdges1[1] - obbHalfEdges1[2];
	corners1[5] = b1.center + obbHalfEdges1[0] - obbHalfEdges1[1] + obbHalfEdges1[2];
	corners1[6] = b1.center + obbHalfEdges1[0] + obbHalfEdges1[1] - obbHalfEdges1[2];
	corners1[7] = b1.center + obbHalfEdges1[0] + obbHalfEdges1[1] + obbHalfEdges1[2];

	// Calculate corners of second OBB
	corners2[0] = b2.center - obbHalfEdges2[0] - obbHalfEdges2[1] - obbHalfEdges2[2];
	corners2[1] = b2.center - obbHalfEdges2[0] - obbHalfEdges2[1] + obbHalfEdges2[2];
	corners2[2] = b2.center - obbHalfEdges2[0] + obbHalfEdges2[1] - obbHalfEdges2[2];
	corners2[3] = b2.center - obbHalfEdges2[0] + obbHalfEdges2[1] + obbHalfEdges2[2];
	corners2[4] = b2.center + obbHalfEdges2[0] - obbHalfEdges2[1] - obbHalfEdges2[2];
	corners2[5] = b2.center + obbHalfEdges2[0] - obbHalfEdges2[1] + obbHalfEdges2[2];
	corners2[6] = b2.center + obbHalfEdges2[0] + obbHalfEdges2[1] - obbHalfEdges2[2];
	corners2[7] = b2.center + obbHalfEdges2[0] + obbHalfEdges2[1] + obbHalfEdges2[2];


	/* =================================== Early return 3: Separating plane along vector between centers ============================== */
	if (!SeparatingAxisTheorem((b1.center - b2.center).normalized(), corners1, 8, corners2, 8))
	{
		return false;
	}


	/* =================================== Separating Axis Theorem using face normals from first obb ================================== */
	for (unsigned int i = 0; i < 3; i++)
	{
		if (!SeparatingAxisTheorem(b1.vectors[i], corners1, 8, corners2, 8))
		{
			return false;
		}
	}

	/* =================================== Separating Axis Theorem using face normals from second obb ================================= */
	for (unsigned int i = 0; i < 3; i++)
	{
		if (!SeparatingAxisTheorem(b2.vectors[i], corners1, 8, corners2, 8))
		{
			return false;
		}
	}

	// The tests above will give a false positive if two edges of the cubes are near each other but not intersecting
	// In other words, when there exists a separating plane, whose normal is not equal to any of the tested obb face normals above

	/* =================================== Separating Axis Theorem using vectors perpendicular to two edges of the obb's ============== */
	for (unsigned int i = 0; i < 3; i++)
	{
		for (unsigned int j = 0; j < 3; j++)
		{
			if (!SeparatingAxisTheorem(b1.vectors[i].crossRH(b2.vectors[j]).normalized(), corners1, 8, corners2, 8))
			{
				return false;
			}
		}
	}

	return true;
}

bool Collision::SeparatingAxisTheorem(const Vector3f & faceNormal, Vector3f * corners1, unsigned int nrOfCorners1, Vector3f * corners2, unsigned int nrOfCorners2)
{
	float distancesFromOriginToProjectedCorners1[8];
	float distancesFromOriginToProjectedCorners2[8];
	float min1, min2, max1, max2;

	min1 = min2 = INFINITY;
	max1 = max2 = -INFINITY;

	// Project each corner of first object onto face normal
	for (unsigned int i = 0; i < nrOfCorners1; i++)
	{
		distancesFromOriginToProjectedCorners1[i] = faceNormal.dot(corners1[i]);

		if (distancesFromOriginToProjectedCorners1[i] < min1)
		{
			min1 = distancesFromOriginToProjectedCorners1[i];
		}
		if (distancesFromOriginToProjectedCorners1[i] > max1)
		{
			max1 = distancesFromOriginToProjectedCorners1[i];
		}
	}

	// Project each corner of second object onto face normal
	for (unsigned int i = 0; i < nrOfCorners2; i++)
	{
		distancesFromOriginToProjectedCorners2[i] = faceNormal.dot(corners2[i]);

		if (distancesFromOriginToProjectedCorners2[i] < min2)
		{
			min2 = distancesFromOriginToProjectedCorners2[i];
		}
		if (distancesFromOriginToProjectedCorners2[i] > max2)
		{
			max2 = distancesFromOriginToProjectedCorners2[i];
		}
	}

	// Check if projected corners overlap
	if (min1 > max2 || min2 > max1)
	{
		return false;
	}

	return true;
}