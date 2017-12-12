#ifndef BIT_MAP_HPP
#define BIT_MAP_HPP

#include "Vector2.hpp"
#include "Vector3.hpp"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Buffer;
struct ID3D11ShaderResourceView;

class BitMap
{
	struct Vertex
	{
		Vector3f position;
		Vector2f uv;
	};

public:
	BitMap();
	~BitMap();

	bool Initialize(ID3D11Device* device, Vector2i screenDimensions, Vector2i bitMapDimensions);
	void SetPosition(Vector2i position);
	void SetupVertexBuffer(ID3D11DeviceContext* deviceContext);
	void SetShaderResourceView(ID3D11ShaderResourceView* shaderResourceView);
	void Render(ID3D11DeviceContext* deviceContext);

	ID3D11ShaderResourceView* GetShaderResourceView();

private:
	ID3D11Buffer* m_vertexBuffer;
	Vertex* m_vertices;
	unsigned int m_nrOfVertices;

	ID3D11ShaderResourceView* m_texture;

	Vector2i m_screenDimensions;
	Vector2i m_bitMapDimensions;
	Vector2i m_previousPosition;
};

#endif