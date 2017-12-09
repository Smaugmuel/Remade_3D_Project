#ifndef RENDER_TEXTURE_HPP
#define RENDER_TEXTURE_HPP
#include "Vector2.hpp"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11ShaderResourceView;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;
struct ID3D11RenderTargetView;

class RenderTexture
{
public:
	RenderTexture();
	~RenderTexture();

	bool Initialize(ID3D11Device* device, const Vector2i& windowDimensions);
	void SetRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView);
	void ClearTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView);
	ID3D11ShaderResourceView* GetShaderResourceView();

private:
	ID3D11Texture2D* m_renderTargetTexture;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11ShaderResourceView* m_shaderResourceView;
};

#endif