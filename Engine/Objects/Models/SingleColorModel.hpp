#ifndef SINGLE_COLOR_MODEL_HPP
#define SINGLE_COLOR_MODEL_HPP
#include "Model.hpp"
#include "../../Math/Vector3.hpp"

struct SingleColorVertex
{
	SingleColorVertex()
	{
	}
	SingleColorVertex(const Vector3f& _position, const Vector3f& _normal, const Vector3f& _color) :
		position(_position),
		normal(_normal),
		color(_color)
	{
	}

	Vector3f position;
	Vector3f normal;
	Vector3f color;
};

class SingleColorModel final : public Model
{
public:
	SingleColorModel(FrameWork* frameWork);
	~SingleColorModel();

	bool LoadFromFile(const char* fileName) final override;
	bool CreateVertexBuffer() final override;
	bool CreateOBB() final override;
	void SetupRender() final override;

private:
	SingleColorVertex* m_vertices;
};

#endif