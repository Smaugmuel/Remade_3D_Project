#ifndef PICKING_RAY_HPP
#define PICKING_RAY_HPP
#include "Ray.hpp"
#include "Vector2.hpp"

class Camera;

namespace HF
{
	Ray CreatePickingRay(Camera* camera, Vector2i mousePosition);
}

#endif