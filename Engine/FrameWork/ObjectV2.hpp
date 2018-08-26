#ifndef OBJECT_V2_HPP
#define OBJECT_V2_HPP
#include "../Math/Vector2.hpp"
#include "../Math/Vector3.hpp"

struct TexturedModelVertexData
{
	Vector3f position;
	Vector3f normal;
	Vector2f uv;
};

struct TexturedModelData
{
	TexturedModelVertexData* vertices;
	int nVerts;
};

struct MaterialData
{
	int textureID;
};

struct TexturedObjectV2
{
	TexturedModelData model;
	MaterialData material;
	int worldMatrix;
};

#endif