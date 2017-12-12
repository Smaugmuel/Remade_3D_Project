#ifndef TEXTURE_MODEL_HPP
#define TEXTURE_MODEL_HPP
#include "Model.hpp"

//#include <d3d11.h>
//#include <DirectXMath.h>
#include "Vector3.hpp"
#include "Vector2.hpp"
//#include <string>

#include <memory>

struct ID3D11Device;
struct ID3D11DeviceContext;

struct TextureVertex
{
	TextureVertex()
	{
	}
	TextureVertex(const Vector3f& _position, const Vector3f& _normal, const Vector2f& _uv) :
		position(_position),
		normal(_normal),
		uv(_uv)
	{
	}

	Vector3f position;
	Vector3f normal;
	Vector2f uv;
};

class TextureModel : public Model
{
public:
	TextureModel();
	~TextureModel();

	bool LoadFromFile(const char* fileName) override;
	bool CreateVertexBuffer(ID3D11Device* device) override;
	void Render(ID3D11DeviceContext* deviceContext) override;

	static Model* Create(ID3D11Device* device, const char* fileName);

private:
	TextureVertex* m_vertices;
	unsigned int m_nrOfVerts;
};

#endif