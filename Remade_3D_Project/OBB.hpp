#ifndef OBB_HPP
#define OBB_HPP
#include "Vector3.hpp"

struct OBB
{
	Vector3f center;
	Vector3f vectors[3];
	float halfSides[3];
};

#endif