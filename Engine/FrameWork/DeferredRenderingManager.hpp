#ifndef DEFERRED_RENDERING_MANAGER_HPP
#define DEFERRED_RENDERING_MANAGER_HPP
#include "../Math/Vector2.hpp"
#include "../Math/Vector3.hpp"

//#define VARYING_TEXTURE_PIXEL_SIZES

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;
struct ID3D11DepthStencilView;
//struct D3D11_VIEWPORT;

class ShaderManagerV2;
class VertexBufferManager;

enum class DeferredBufferTypes
{
	WORLD_POSITIONS,
	NORMALS,
	COLORS
};

static const unsigned int NR_OF_DEFERRED_OUTPUT_BUFFERS = static_cast<unsigned int>(DeferredBufferTypes::COLORS) + 1U;

class DeferredRenderingManager final
{
public:
	DeferredRenderingManager();
	~DeferredRenderingManager();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const Vector2i& windowDimensions, VertexBufferManager* vertexBufferManager, ShaderManagerV2* shaderManager);

	void RenderLightPass();

	void ClearRenderTargets(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
	void SetRenderTargets();

	ID3D11ShaderResourceView** GetShaderResourceViews();

private:
	struct Vertex
	{
		Vector3f position;
		Vector2f uv;
	};

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;

	/*
	Resources for first pass
	*/
	ID3D11RenderTargetView* m_renderTargetViews[NR_OF_DEFERRED_OUTPUT_BUFFERS];
	ID3D11ShaderResourceView* m_shaderResourceViews[NR_OF_DEFERRED_OUTPUT_BUFFERS];
	ID3D11DepthStencilView* m_depthStencilView;

	//D3D11_VIEWPORT* m_viewPort;

	/*
	Resources for light pass
	*/
	Vertex m_vertices[3];
	int m_vertexBufferID;
	ShaderManagerV2* m_shaderManager;
	VertexBufferManager* m_vertexBufferManager;
};

#endif