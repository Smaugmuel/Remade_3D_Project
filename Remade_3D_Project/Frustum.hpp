#ifndef FRUSTUM_HPP
#define FRUSTUM_HPP
#include <DirectXMath.h>
#include "Plane.hpp"
#include "AABB.hpp"

struct Frustum
{
	void Create(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection);

	Plane planes[6];
};

void FrustumCorners(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection, Vector3f* cornerArray);
AABB FrustumAABB(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection);

#endif