#include "DeferredScreenTarget.hpp"
#include "FrameWorkManager.hpp"

DeferredScreenTarget::DeferredScreenTarget() : m_vertexBufferManager(nullptr)
{
}

DeferredScreenTarget::~DeferredScreenTarget()
{
}

bool DeferredScreenTarget::Initialize(FrameWorkManager* frameWorkManager)
{
	m_frameWorkManager = frameWorkManager;
	m_vertexBufferManager = m_frameWorkManager->GetVertexBufferManager();

	/* Use an entire quad */
	/*m_vertices[0].position = Vector3f(-1.0f, -1.0f, 0.0f);
	m_vertices[1].position = Vector3f(-1.0f, 1.0f, 0.0f);
	m_vertices[2].position = Vector3f(1.0f, 1.0f, 0.0f);
	m_vertices[3].position = Vector3f(1.0f, 1.0f, 0.0f);
	m_vertices[4].position = Vector3f(1.0f, -1.0f, 0.0f);
	m_vertices[5].position = Vector3f(-1.0f, -1.0f, 0.0f);
	m_vertices[0].uv = Vector2f(0.0f, 1.0f);
	m_vertices[1].uv = Vector2f(0.0f, 0.0f);
	m_vertices[2].uv = Vector2f(1.0f, 0.0f);
	m_vertices[3].uv = Vector2f(1.0f, 0.0f);
	m_vertices[4].uv = Vector2f(1.0f, 1.0f);
	m_vertices[5].uv = Vector2f(0.0f, 1.0f);*/

	/* Use only a triangle and not a quad */
	m_vertices[0].position = Vector3f(-3.0f, -1.0f, 0.0f);	// bottom left
	m_vertices[1].position = Vector3f(1.0f, 3.0f, 0.0f);	// upper right
	m_vertices[2].position = Vector3f(1.0f, -1.0f, 0.0f);	// bottom right
	m_vertices[0].uv = Vector2f(-1.0f, 1.0f);
	m_vertices[1].uv = Vector2f(1.0f, -1.0f);
	m_vertices[2].uv = Vector2f(1.0f, 1.0f);

	/* Cut screen in half */
	//m_vertices[0].position = Vector3f(-1.0f, -1.0f, 0.0f);	// bottom left
	//m_vertices[1].position = Vector3f(1.0f, 1.0f, 0.0f);	// upper right
	//m_vertices[2].position = Vector3f(1.0f, -1.0f, 0.0f);	// bottom right
	//m_vertices[0].uv = Vector2f(0.0f, 1.0f);
	//m_vertices[1].uv = Vector2f(1.0f, 0.0f);
	//m_vertices[2].uv = Vector2f(1.0f, 1.0f);

	unsigned int vertexSize = sizeof(Vertex);

	m_vertexBufferID = m_vertexBufferManager->CreateBuffer(vertexSize * 3, vertexSize, m_vertices);
	if (m_vertexBufferID == -1)
		return false;

	return true;
}

void DeferredScreenTarget::Render()
{
	unsigned int nrOfVertices = sizeof(m_vertices) / sizeof(Vertex);

	m_frameWorkManager->GetShaderManager()->SetShaders(ShaderTypeV2::LIGHT_PASS);
	//m_frameWorkManager->ClearLightPassRenderTargets();	// This is cleared in Engine.cpp at the same time as first pass
	m_frameWorkManager->SetLightPassRenderTarget();
	m_vertexBufferManager->SetBufferToInputAssembler(m_vertexBufferID, 0);
	


	/*
	This and Direct3D should be changed to store the deferred textures in the texture manager instead,
	then utilizing it when setting a texture to the shader
	*/
	//m_frameWorkManager->GetTextureManager()->SetTextureToPixelShader(m_frameWorkManager->GetTextureManager()->LoadTexture("Missing.jpg"), 2);
	ID3D11ShaderResourceView** deferredTextures = m_frameWorkManager->GetDirect3D()->GetDeferredShaderResourceViews();
	m_frameWorkManager->GetDirect3D()->GetDeviceContext()->PSSetShaderResources(0, 3, deferredTextures);
	



	m_frameWorkManager->RenderWithCurrentSettings(nrOfVertices);
}