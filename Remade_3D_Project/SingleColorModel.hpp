#ifndef SINGLE_COLOR_MODEL_HPP
#define SINGLE_COLOR_MODEL_HPP
#include "Model.hpp"
#include "Vector3.hpp"

struct ID3D11Device;
struct ID3D11DeviceContext;

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

class SingleColorModel : public Model
{
public:
	SingleColorModel();
	~SingleColorModel();

	bool LoadFromFile(const char* fileName) override;
	bool CreateVertexBuffer(ID3D11Device* device) override;
	bool CreateOBB() override;
	void SetupRender(ID3D11DeviceContext* deviceContext) override;

private:
	SingleColorVertex* m_vertices;
};

#endif