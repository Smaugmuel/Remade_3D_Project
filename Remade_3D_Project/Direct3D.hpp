#ifndef DIRECT_3D_HPP
#define DIRECT_3D_HPP

#include "Singleton.hpp"
#include "Vector2.hpp"
#include "Window.hpp"
#include <vector>

#include <d3d11.h>
//#include <d3dcompiler.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

enum BufferType
{
	//DEFAULT,
	D_WORLD_POS,		// Deferred
	D_NORMALS,
	D_COLOR,
	NR_OF_D_ELEMENTS,
	//S_DEPTH				// Shadow
};

class Direct3D final : public Singleton<Direct3D>
{
	friend class Singleton<Direct3D>;

	Direct3D();
	~Direct3D();

public:
	bool Initialize(const HWND& windowHandle, const Vector2i& dimensions);
	bool Initialize(Window* window);

	void ClearDefaultTarget();
	void ClearDeferredTargets();
	//void ClearRenderToTextureTarget();
	void ClearShadowTarget();
	void ClearAllTargets();

	void Present();

	void SetDefaultTarget();
	void SetDeferredTargets();
	//void SetRenderToTextureTarget();
	void SetShadowTarget();

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;


	/* Added with rastertek */
	ID3D11DepthStencilView* GetDepthStencilView();
	void EnableZBuffer();
	void DisableZBuffer();


	ID3D11ShaderResourceView** GetDeferredShaderResourceViews();
	//ID3D11ShaderResourceView* GetRenderToTextureShaderResoureView();
	ID3D11ShaderResourceView* GetShadowShaderResourceView();

private:
	bool InitializeDeviceAndSwapChain();
	bool InitializeDefaultRenderTargetView();
	bool InitializeDefaultDepthBufferAndDepthStencilView();
	bool InitializeDefaultViewport();

	// Deferred
	bool InitializeDeferredRenderTargetViews();
	bool InitializeDeferredDepthBufferAndDepthStencilView();
	bool InitializeDeferredViewport();

	/* Render to texture */
	//bool InitializeRenderToTextureRenderTargetView();

	/* Shadow */
	bool InitializeShadowDepthBufferAndDepthStencilView();
	bool InitializeShadowShaderResourceView();
	bool InitializeShadowViewport();

	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilState* m_depthDisabledStencilState;	/* Added with rastertek */
	ID3D11DepthStencilView* m_depthStencilView;
	D3D11_VIEWPORT m_viewPort;

	// Deferred
	ID3D11Texture2D* m_d_renderTargetTextures[BufferType::NR_OF_D_ELEMENTS];
	ID3D11RenderTargetView* m_d_renderTargetViews[BufferType::NR_OF_D_ELEMENTS];
	ID3D11ShaderResourceView* m_d_shaderResourceViews[BufferType::NR_OF_D_ELEMENTS];
	ID3D11Texture2D* m_d_depthStencilBuffer;
	ID3D11DepthStencilView* m_d_depthStencilView;
	D3D11_VIEWPORT m_d_viewPort;

	/* Render to texture */
	//ID3D11Texture2D* m_rt_renderTargetTexture;
	//ID3D11RenderTargetView* m_rt_renderTargetView;
	//ID3D11ShaderResourceView* m_rt_shaderResourceView;

	/* Shadow */
	ID3D11ShaderResourceView* m_s_shaderResourceView;
	ID3D11Texture2D* m_s_depthStencilBuffer;
	ID3D11DepthStencilView* m_s_depthStencilView;
	D3D11_VIEWPORT m_s_viewport;

	HWND m_windowHandle;
	Vector2i m_windowDimensions;
};

#endif