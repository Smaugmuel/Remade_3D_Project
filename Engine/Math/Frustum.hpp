#ifndef FRUSTUM_HPP
#define FRUSTUM_HPP
#include "Plane.hpp"

struct Frustum
{
	Plane planes[6];
};

//void FrustumCorners(const Matrix& view, const Matrix& projection, Vector3f* cornerArray);
//AABB FrustumAABB(const Matrix& view, const Matrix& projection);

#endif