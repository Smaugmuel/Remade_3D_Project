#include "OnScreenTarget.hpp"
#include "VertexBufferManager.hpp"

OnScreenTarget::OnScreenTarget()
{
}

OnScreenTarget::~OnScreenTarget()
{
}


bool OnScreenTarget::Initialize(VertexBufferManager * vertexBufferManager)
{
	m_vertexBufferManager = vertexBufferManager;
	
	/* A triangle perfectly covering the screen */
	m_vertices[0].position = Vector3f(-3.0f, -1.0f, 0.0f);	// bottom left
	m_vertices[1].position = Vector3f(1.0f, 3.0f, 0.0f);	// upper right
	m_vertices[2].position = Vector3f(1.0f, -1.0f, 0.0f);	// bottom right
	m_vertices[0].uv = Vector2f(-1.0f, 1.0f);
	m_vertices[1].uv = Vector2f(1.0f, -1.0f);
	m_vertices[2].uv = Vector2f(1.0f, 1.0f);

	/* A triangle covering half the screen */
	//m_vertices[0].position = Vector3f(-1.0f, -1.0f, 0.0f);	// bottom left
	//m_vertices[1].position = Vector3f(1.0f, 1.0f, 0.0f);	// upper right
	//m_vertices[2].position = Vector3f(1.0f, -1.0f, 0.0f);	// bottom right
	//m_vertices[0].uv = Vector2f(0.0f, 1.0f);
	//m_vertices[1].uv = Vector2f(1.0f, 0.0f);
	//m_vertices[2].uv = Vector2f(1.0f, 1.0f);

	m_vertexBufferID = m_vertexBufferManager->CreateBuffer(sizeof(Vertex) * 3, sizeof(Vertex), m_vertices);
	if (m_vertexBufferID == -1)
		return false;

	return true;
}

void OnScreenTarget::SetAsTarget()
{
	m_vertexBufferManager->SetBufferToInputAssembler(m_vertexBufferID, 0);
}