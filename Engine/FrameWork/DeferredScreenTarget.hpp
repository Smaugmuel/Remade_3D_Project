#ifndef DEFERRED_SCREEN_TARGET_HPP
#define DEFERRED_SCREEN_TARGET_HPP
#include "../Math/Vector2.hpp"
#include "../Math/Vector3.hpp"

class FrameWorkManager;
class VertexBufferManager;

class DeferredScreenTarget final
{
public:
	DeferredScreenTarget();
	~DeferredScreenTarget();

	bool Initialize(FrameWorkManager* frameWorkManager);
	void Render();

private:
	struct Vertex
	{
		Vector3f position;
		Vector2f uv;
	};

	Vertex m_vertices[3];
	int m_vertexBufferID;

	FrameWorkManager* m_frameWorkManager;
	VertexBufferManager* m_vertexBufferManager;
};

#endif