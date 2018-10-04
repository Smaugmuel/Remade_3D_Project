#include "Math.hpp"
#include <DirectXMath.h>
#include <cstring>	// Only included for std::memcpy

Math::FrustumPlanes Math::CalculatePlanesAroundFrustum(const Matrix & vp)
{
	Math::FrustumPlanes frustum;
	// Top plane
	frustum.planes[0].normal = Vector3f(vp.rows[0][3] - vp.rows[0][1], vp.rows[1][3] - vp.rows[1][1], vp.rows[2][3] - vp.rows[2][1]) * -1;
	// Bottom plane
	frustum.planes[1].normal = Vector3f(vp.rows[0][3] + vp.rows[0][1], vp.rows[1][3] + vp.rows[1][1], vp.rows[2][3] + vp.rows[2][1]) * -1;
	// Left plane
	frustum.planes[2].normal = Vector3f(vp.rows[0][3] + vp.rows[0][0], vp.rows[1][3] + vp.rows[1][0], vp.rows[2][3] + vp.rows[2][0]) * -1;
	// Right plane
	frustum.planes[3].normal = Vector3f(vp.rows[0][3] - vp.rows[0][0], vp.rows[1][3] - vp.rows[1][0], vp.rows[2][3] - vp.rows[2][0]) * -1;
	// Near plane
	frustum.planes[4].normal = Vector3f(vp.rows[0][3] + vp.rows[0][2], vp.rows[1][3] + vp.rows[1][2], vp.rows[2][3] + vp.rows[2][2]) * -1;
	// Far plane
	frustum.planes[5].normal = Vector3f(vp.rows[0][3] - vp.rows[0][2], vp.rows[1][3] - vp.rows[1][2], vp.rows[2][3] - vp.rows[2][2]) * -1;

	frustum.planes[0].d = -(vp.rows[3][3] - vp.rows[3][1]);
	frustum.planes[1].d = -(vp.rows[3][3] + vp.rows[3][1]);
	frustum.planes[3].d = -(vp.rows[3][3] + vp.rows[3][0]);
	frustum.planes[2].d = -(vp.rows[3][3] - vp.rows[3][0]);
	frustum.planes[5].d = -(vp.rows[3][3] + vp.rows[3][2]);
	frustum.planes[4].d = -(vp.rows[3][3] - vp.rows[3][2]);

	// Normalize plane
	for (int i = 0; i < 6; i++)
	{
		float invLength = 1.0f / frustum.planes[i].normal.length();

		/* Vector::Normalize could be used here,
		but since the inverse length needs to be calculated for the float anyway,
		it would require twice the amount of square roots and divisions */
		frustum.planes[i].normal.x *= invLength;
		frustum.planes[i].normal.y *= invLength;
		frustum.planes[i].normal.z *= invLength;
		frustum.planes[i].d *= invLength;
	}


	/*Frustum frustum2;
	DirectX::XMMATRIX xmViewProj;
	DirectX::XMFLOAT4X4 matrix;
	float invLength;

	std::memcpy(xmViewProj.r, vp.rows, 64U);

	DirectX::XMStoreFloat4x4(&matrix, xmViewProj);

	// Top clipping plane
	frustum2.planes[0].normal.x = -(matrix._14 - matrix._12);
	frustum2.planes[0].normal.y = -(matrix._24 - matrix._22);
	frustum2.planes[0].normal.z = -(matrix._34 - matrix._32);
	frustum2.planes[0].d = -(matrix._44 - matrix._42);

	// Bottom clipping plane
	frustum2.planes[1].normal.x = -(matrix._14 + matrix._12);
	frustum2.planes[1].normal.y = -(matrix._24 + matrix._22);
	frustum2.planes[1].normal.z = -(matrix._34 + matrix._32);
	frustum2.planes[1].d = -(matrix._44 + matrix._42);

	// Left clipping plane
	frustum2.planes[2].normal.x = -(matrix._14 + matrix._11);
	frustum2.planes[2].normal.y = -(matrix._24 + matrix._21);
	frustum2.planes[2].normal.z = -(matrix._34 + matrix._31);
	frustum2.planes[2].d = -(matrix._44 + matrix._41);

	// Right clipping plane
	frustum2.planes[3].normal.x = -(matrix._14 - matrix._11);
	frustum2.planes[3].normal.y = -(matrix._24 - matrix._21);
	frustum2.planes[3].normal.z = -(matrix._34 - matrix._31);
	frustum2.planes[3].d = -(matrix._44 - matrix._41);

	// Near clipping plane
	frustum2.planes[4].normal.x = -(matrix._14 + matrix._13);
	frustum2.planes[4].normal.y = -(matrix._24 + matrix._23);
	frustum2.planes[4].normal.z = -(matrix._34 + matrix._33);
	frustum2.planes[4].d = -(matrix._44 + matrix._43);

	// Far clipping plane
	frustum2.planes[5].normal.x = -(matrix._14 - matrix._13);
	frustum2.planes[5].normal.y = -(matrix._24 - matrix._23);
	frustum2.planes[5].normal.z = -(matrix._34 - matrix._33);
	frustum2.planes[5].d = -(matrix._44 - matrix._43);

	// Normalize plane
	for (int i = 0; i < 6; i++)
	{
		invLength = 1.0f / frustum2.planes[i].normal.length();

		// Vector::Normalize could be used,
		// but since the inverse length needs to be calculated for the float anyway,
		// it would require twice the amount of square roots and divisions
		frustum2.planes[i].normal.x *= invLength;
		frustum2.planes[i].normal.y *= invLength;
		frustum2.planes[i].normal.z *= invLength;
		frustum2.planes[i].d *= invLength;
	}*/


	return frustum;
}

Math::FrustumPlanes Math::CalculatePlanesAroundFrustum(const Matrix & view, const Matrix & projection)
{
	return Math::CalculatePlanesAroundFrustum(view * projection);
}

Math::FrustumCorners Math::CalculateCornersOfFrustum(const Matrix& view, const Matrix& proj)
{
	Math::FrustumCorners frustumCorners;
	DirectX::XMMATRIX xmView, xmProj, inverse;
	DirectX::XMVECTOR corners[8], determinant;

	// Convert matrices to directX matrices
	std::memcpy(xmView.r, view.rows, 64U);
	std::memcpy(xmProj.r, proj.rows, 64U);

	// Corners in NDC space
	corners[0] = { -1, -1, 0, 1 };
	corners[1] = { -1, -1, 1, 1 };
	corners[2] = { -1,  1, 0, 1 };
	corners[3] = { -1,  1, 1, 1 };
	corners[4] = {  1, -1, 0, 1 };
	corners[5] = {  1, -1, 1, 1 };
	corners[6] = {  1,  1, 0, 1 };
	corners[7] = {  1,  1, 1, 1 };

	//// Transformation test from world to screen
	//DirectX::XMVECTOR originWorldSpace = { 0.0f, 0.0f, 0.0f, 1 };
	//DirectX::XMVECTOR originViewSpace = DirectX::XMVector4Transform(originWorldSpace, xmView);
	//DirectX::XMVECTOR originProjSpace = DirectX::XMVector4Transform(originViewSpace, xmProj);
	//DirectX::XMVECTOR originNDCSpace = originProjSpace;
	//
	//// Convert to [(-1, -1, 0), (1, 1, 1)]
	//originNDCSpace.m128_f32[0] /= originNDCSpace.m128_f32[3];
	//originNDCSpace.m128_f32[1] /= originNDCSpace.m128_f32[3];
	//originNDCSpace.m128_f32[2] /= originNDCSpace.m128_f32[3];
	//// The z-value is non-linear, being ~0.48 when distance is ~0.5 and ~1 when distance is ~1000


	// Convert corners to view space
	determinant = DirectX::XMMatrixDeterminant(xmProj);
	inverse = DirectX::XMMatrixInverse(&determinant, xmProj);
	for (unsigned int i = 0; i < 8; i++)
	{
		corners[i] = DirectX::XMVector4Transform(corners[i], inverse);
	}

	// Convert corners to world space
	determinant = DirectX::XMMatrixDeterminant(xmView);
	inverse = DirectX::XMMatrixInverse(&determinant, xmView);
	for (unsigned int i = 0; i < 8; i++)
	{
		corners[i] = DirectX::XMVector4Transform(corners[i], inverse);

		frustumCorners.corners[i] = Vector3f(corners[i].m128_f32[0], corners[i].m128_f32[1], corners[i].m128_f32[2]);
		frustumCorners.corners[i] /= corners[i].m128_f32[3];
	}

	return frustumCorners;
}

Math::Ray Math::CalculatePickingRay(const Matrix & view, const Matrix & projection, Vector2i mousePosition, Vector2i windowDimensions)
{
	Math::FrustumCorners frustumCorners;
	Math::Ray ray;

	// The 8 corners of frustum in world coordinates
	frustumCorners = Math::CalculateCornersOfFrustum(view, projection);

	// Mouse coordinates in percent, in range [(0, 0), (1, 1)]
	Vector2f mousePercentage(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
	mousePercentage.x /= windowDimensions.x;
	mousePercentage.y /= windowDimensions.y;
	mousePercentage.y = 1 - mousePercentage.y;

	Vector3f mouseDirectionOnFarPlaneX = (frustumCorners.corners[5] - frustumCorners.corners[1]) * mousePercentage.x;
	Vector3f mouseDirectionOnFarPlaneY = (frustumCorners.corners[3] - frustumCorners.corners[1]) * mousePercentage.y;
	Vector3f mouseDirectionOnNearPlaneX = (frustumCorners.corners[4] - frustumCorners.corners[0]) * mousePercentage.x;
	Vector3f mouseDirectionOnNearPlaneY = (frustumCorners.corners[2] - frustumCorners.corners[0]) * mousePercentage.y;

	Vector3f mousePositionOnFarPlane = (frustumCorners.corners[1] + mouseDirectionOnFarPlaneX + mouseDirectionOnFarPlaneY);
	Vector3f mousePositionOnNearPlane = (frustumCorners.corners[0] + mouseDirectionOnNearPlaneX + mouseDirectionOnNearPlaneY);

	ray.direction = (mousePositionOnFarPlane - mousePositionOnNearPlane).normalized();
	ray.origin = mousePositionOnNearPlane;

	return ray;
}

Vector3f Math::Transform(const Vector3f & v, float w, const Matrix & m)
{
	DirectX::XMMATRIX xm;
	DirectX::XMVECTOR xv = { v.x, v.y, v.z, w };
	
	std::memcpy(xm.r, m.rows, 64U);
	xv = DirectX::XMVector4Transform(xv, xm);

	return Vector3f(xv.m128_f32[0], xv.m128_f32[1], xv.m128_f32[2]);
}

Math::Collision::RayIntersection Math::Collision::RayVsSphere(const Ray & ray, const Sphere & sphere)
{
	Math::Collision::RayIntersection intersection;

	// Vector from the ray origin to the sphere center
	Vector3f toCenter = sphere.position - ray.origin;

	// Distance of vector, from the ray origin to the sphere center, projected on the ray direction
	float s = toCenter.dot(ray.direction);

	// Squared distance from the ray origin to the sphere center (for better performance)
	float distToCenterSquared = toCenter.lengthSquared();

	// If the ray direction is away from the sphere AND the ray origin is outside of the sphere, there is no collision
	if (s < 0 && distToCenterSquared > sphere.radiusSquared)
	{
		return intersection;
	}

	// Squared distance from closest point on the ray to the sphere center
	float m2 = distToCenterSquared - s * s;

	// If the ray misses the sphere, there is no collision
	if (m2 > sphere.radiusSquared)
	{
		return intersection;
	}

	// At this point, a collision did occur
	intersection.collisionOccured = true;

	// Distance along the ray, from the point on the ray closest to the sphere center, to the sphere hull
	float q = sqrtf(sphere.radiusSquared - m2);

	// Determine which intersection point is the closest
	if (distToCenterSquared > sphere.radiusSquared)
	{
		// The ray origin is outside the sphere, pointing towards it
		intersection.distance = s - q;
	}
	else
	{
		// The ray origin is inside the sphere, pointing out from it
		intersection.distance = s + q;
	}

	return intersection;
}

Math::Collision::RayIntersection Math::Collision::RayVsOBB(const Ray & ray, const OBB & obb)
{
	Math::Collision::RayIntersection intersection;

	float t_min = -INFINITY;
	float t_max = INFINITY;

	Vector3f toCenter = obb.center - ray.origin;

	for (unsigned int i = 0; i < 3; i++)
	{
		// Distance from ray origin to obb center projected on each obb face normal
		float e = obb.vectors[i].dot(toCenter);

		// Cosine angle between ray direction and obb face normal
		float cos_v = obb.vectors[i].dot(ray.direction);

		// Check if the cosine angle is close to 0
		// If it's not, the ray direction is not perpendicular to the obb face normal,
		// (a.k.a. not parallel to the corresponding face,)
		// and then an intersection can be found
		if (fabsf(cos_v) > 0.000001f)
		{
			// Distances, along obb's face normal, from ray origin to its edges
			float near = (e + obb.halfSides[i]);
			float far = (e - obb.halfSides[i]);

			// Temporary variable, to optimize
			float div_cos_v = 1.0f / cos_v;

			// Find actual length of vector from ray origin to intersections
			// This uses trigonometry: cos(v) = adjacent / hypotenuse
			// Hypothenuse = adjacent / cos(v)
			near *= div_cos_v;
			far *= div_cos_v;

			// Ensure values are ordered correctly
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
				return intersection;
			}

			// Check if ray origin is in front of intersection
			if (t_max < 0)
			{
				return intersection;
			}
		}

		// Check if ray would go past obb
		else if (fabsf(e) > obb.halfSides[i])
		{
			return intersection;
		}
	}

	// Check if origin is not inside obb
	intersection.distance = t_min > 0 ? t_min : t_max;

	return intersection;
}

bool Math::Collision::SphereVsOBB(const Math::Sphere & sphere, const OBB & obb)
{
	float d[3];
	Vector3f obbToSphere = sphere.position - obb.center;

	// The length of the vector above when projected on each of the obb's vectors
	// The signed value represents direction
	d[0] = obbToSphere.dot(obb.vectors[0]);
	d[1] = obbToSphere.dot(obb.vectors[1]);
	d[2] = obbToSphere.dot(obb.vectors[2]);

	// Saturate the distances to prevent the point from being outside the obb
	d[0] = fmaxf(fminf(d[0], obb.halfSides[0]), -obb.halfSides[0]);
	d[1] = fmaxf(fminf(d[1], obb.halfSides[1]), -obb.halfSides[1]);
	d[2] = fmaxf(fminf(d[2], obb.halfSides[2]), -obb.halfSides[2]);

	// The closest point is the sum of the above distances, each along the axises of the obb,
	// from the center of the obb
	Vector3f closestPointOnOBB =
		obb.center +
		obb.vectors[0] * d[0] +
		obb.vectors[1] * d[1] +
		obb.vectors[2] * d[2];

	return (sphere.position - closestPointOnOBB).lengthSquared() < sphere.radiusSquared;
}
