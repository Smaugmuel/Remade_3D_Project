#ifndef HUD_TEXTURE_HPP
#define HUD_TEXTURE_HPP
#include "../Math/Vector2.hpp"
#include "../Math/Vector3.hpp"

#include <memory>
#include <string>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Buffer;
struct ID3D11ShaderResourceView;

//class Texture;

class HUDTexture
{
	struct Vertex
	{
		Vector3f position;
		Vector2f uv;
	};

public:
	HUDTexture();
	~HUDTexture();

	bool Initialize(ID3D11Device* device, const std::string& textureName, Vector2i position, Vector2i dimensions);
	bool InitializeVertexBuffer(ID3D11Device* device);
	
	void SetPosition(Vector2i position);
	void SetDimensions(Vector2i dimensions);
	void SetShaderResourceView(ID3D11ShaderResourceView* shaderResourceView);
	
	void SetDefaultShaderResourceView();

	void SetupVertexBuffer(ID3D11DeviceContext* deviceContext);
	void Render(ID3D11DeviceContext* deviceContext);

	ID3D11ShaderResourceView* GetShaderResourceView();

private:
	void UpdateVertices();

	ID3D11Buffer* m_vertexBuffer;
	Vertex* m_vertices;
	unsigned int m_nrOfVertices;

	//std::unique_ptr<Texture> m_texture;
	std::string m_textureName;
	ID3D11ShaderResourceView* m_shaderResourceView;

	Vector2i m_dimensions;
	Vector2i m_position;

	bool m_vertices_changed_flag;
};

#endif