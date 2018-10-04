#ifndef OBB_HPP
#define OBB_HPP
#include "Vector3.hpp"

struct OBB
{
	Vector3f center;
	Vector3f vectors[3] = { Vector3f(1, 0, 0), Vector3f(0, 1, 0), Vector3f(0, 0, 1) };
	float halfSides[3] = { 1, 1, 1 };
	float defaultHalfSides[3] = { 0, 0, 0 };
};

#endif