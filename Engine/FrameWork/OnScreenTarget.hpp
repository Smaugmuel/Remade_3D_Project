#ifndef ON_SCREEN_TARGET_HPP
#define ON_SCREEN_TARGET_HPP
#include "../Math/Vector2.hpp"
#include "../Math/Vector3.hpp"

class VertexBufferManager;

class OnScreenTarget
{
public:
	OnScreenTarget();
	~OnScreenTarget();

	bool Initialize(VertexBufferManager* vertexBufferManager);

	void SetAsTarget();

private:
	struct Vertex
	{
		Vector3f position;
		Vector2f uv;
	};

	Vertex m_vertices[3];
	int m_vertexBufferID;
	VertexBufferManager* m_vertexBufferManager;

};

#endif