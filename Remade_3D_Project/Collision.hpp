#ifndef COLLISION_HPP
#define COLLISION_HPP
#include "Singleton.hpp"

#include "Vector3.hpp"
#include "Ray.hpp"
#include "AABB.hpp"
#include "OBB.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Frustum.hpp"

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
IntersectionData RayVsOBB(const Ray& ray, const OBB& obb);
IntersectionData RayVSSphere(const Vector3f& origin, const Vector3f& direction, const Sphere& sphere);
IntersectionData RayVSSphere(const Ray& ray, const Sphere& sphere);

class Collision : public Singleton<Collision>
{
	friend class Singleton<Collision>;
	Collision();
	~Collision();

public:
	bool FrustumVSOBB(const Frustum& frustum, const OBB& obb);
	bool FrustumVSPoint(const Frustum& frustum, Vector3f point);

	float DistancePlaneToPoint(const Plane& plane, Vector3f point);
};

#endif