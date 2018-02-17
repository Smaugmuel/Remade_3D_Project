#ifndef CONSTANT_BUFFER_TYPES_HPP
#define CONSTANT_BUFFER_TYPES_HPP
#include "SystemInformation.hpp"
#include <DirectXMath.h>

struct Float4
{
	float f[4];
};

struct Int4
{
	int i[4];
};

struct Float4Array
{
	Float4 floats[MAX_NR_OF_LIGHTS];
};

struct Matrix
{
	DirectX::XMMATRIX matrix;
};

struct Matrix2
{
	Matrix matrices[2];
};

#endif