#ifndef MATH_HPP
#define MATH_HPP
#include "Matrix.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
//#include "Ray.hpp"
//#include "Plane.hpp"
//#include "Frustum.hpp"
//#include "OBB.hpp"
#include "AABB.hpp"
#include "AABA.hpp"

namespace Math
{
	struct Plane
	{
		Vector3f normal;
		float d = 0.0f;
	};
	struct FrustumPlanes
	{
		Plane planes[6];
	};
	struct FrustumCorners
	{
		Vector3f corners[8];
	};
	struct Sphere
	{
		Vector3f position;
		float radius = 1.0f;
		float radiusSquared = 1.0f;
	};
	struct OBB
	{
		Vector3f center;
		Vector3f vectors[3] = { Vector3f(1, 0, 0), Vector3f(0, 1, 0), Vector3f(0, 0, 1) };
		float halfSides[3] = { 1, 1, 1 };
		float defaultHalfSides[3] = { 0, 0, 0 };
	};
	struct Ray
	{
		Vector3f origin;
		Vector3f direction;
	};
	
	Math::FrustumPlanes CalculatePlanesAroundFrustum(const Matrix& viewProjection);
	Math::FrustumPlanes CalculatePlanesAroundFrustum(const Matrix& view, const Matrix& projection);
	Math::FrustumCorners CalculateCornersOfFrustum(const Matrix& view, const Matrix& projection);
	Math::Ray CalculatePickingRay(const Matrix& view, const Matrix& projection, Vector2i mousePosition, Vector2i windowDimensions);
	Vector3f Transform(const Vector3f& vector, float wValue, const Matrix& transformationMatrix);


	namespace Collision
	{
		struct RayIntersection
		{
			bool collisionOccured = false;
			float distance = 0.0f;
		};


		RayIntersection RayVsSphere(const Ray& ray, const Sphere& sphere);
		RayIntersection RayVsOBB(const Ray& ray, const OBB& obb);
		bool SphereVsOBB(const Math::Sphere& sphere, const OBB& obb);
	}
}

#endif