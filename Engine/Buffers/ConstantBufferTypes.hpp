#ifndef CONSTANT_BUFFER_TYPES_HPP
#define CONSTANT_BUFFER_TYPES_HPP
#include "../Lights/LightSettings.hpp"
#include "../Render/ChunkRenderSettings.hpp"
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
	DirectX::XMMATRIX matrices[2];
};

struct MatrixArray
{
	DirectX::XMMATRIX matrices[MAX_NR_OF_LIGHTS];
};

struct MatrixChunk
{
	DirectX::XMMATRIX matrices[CHUNK_SIZE];
};

#endif