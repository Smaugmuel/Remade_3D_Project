#ifndef TEXTURE_MODEL_HPP
#define TEXTURE_MODEL_HPP
#include "Model.hpp"

#include "../../Math/Vector2.hpp"
#include "../../Math/Vector3.hpp"

//#include <string>
#include <memory>

class TextureModel final : public Model
{
public:
	TextureModel();
	~TextureModel();

	bool LoadFromFile(const char* fileName) final override;
	bool CreateVertexBuffer() final override;
	bool CreateOBB() final override;
	void SetupRender() final override;

	//const std::string& GetMaterialName() const;
	int GetMaterialIndex() const;

private:
	struct TextureVertex
	{
		TextureVertex() { }
		TextureVertex(Vector3f _position, Vector3f _normal, Vector2f _uv) :
			position(_position), normal(_normal), uv(_uv) { }

		Vector3f position;
		Vector3f normal;
		Vector2f uv;
	};

	TextureVertex* m_vertices;
	int m_materialIndex;
	//std::string m_materialName;
};

#endif