#ifndef DEFERRED_RENDERING_MANAGER_HPP
#define DEFERRED_RENDERING_MANAGER_HPP
#include "../Math/Vector2.hpp"

//#define VARYING_TEXTURE_PIXEL_SIZES

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;
struct ID3D11DepthStencilView;
//struct D3D11_VIEWPORT;

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

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const Vector2i& windowDimensions);

	void RenderLightPass();

	void ClearGeometryRenderTargets(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
	void ClearLightRenderTarget(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);

	void SetGeometryRenderTargets();
	void SetLightRenderTarget();

	ID3D11ShaderResourceView** GetGeometryPassShaderResourceViews();
	ID3D11ShaderResourceView* GetLightPassShaderResourceView();

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;

	/*
	Resources for geometry pass
	*/
	ID3D11RenderTargetView* m_gPassRTVs[NR_OF_DEFERRED_OUTPUT_BUFFERS];
	ID3D11ShaderResourceView* m_gPassSRVs[NR_OF_DEFERRED_OUTPUT_BUFFERS];
	ID3D11DepthStencilView* m_gPassDSV;

	/*
	Resources for light pass
	*/
	ID3D11RenderTargetView* m_lPassRTV;
	ID3D11ShaderResourceView* m_lPassSRV;
	ID3D11DepthStencilView* m_lPassDSV;
};

#endif