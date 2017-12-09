#ifndef DEBUG_WINDOW_HPP
#define DEBUG_WINDOW_HPP

#include "Vector2.hpp"
#include "Vector3.hpp"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Buffer;

class DebugWindow
{
	struct Vertex
	{
		Vector3f position;
		Vector2f uv;
	};

public:
	DebugWindow();
	~DebugWindow();

	bool Initialize(ID3D11Device* device, Vector2i screenDimensions, Vector2i bitMapDimensions);
	void SetPosition(Vector2i position);
	void SetupVertexBuffer(ID3D11DeviceContext* deviceContext);
	void Render(ID3D11DeviceContext* deviceContext);

private:
	ID3D11Buffer* m_vertexBuffer;
	Vertex* m_vertices;
	unsigned int m_nrOfVertices;
	
	Vector2i m_screenDimensions;
	Vector2i m_bitMapDimensions;
	Vector2i m_previousPosition;
};

#endif