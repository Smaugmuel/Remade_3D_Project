#ifndef DIRECT_3D_HPP
#define DIRECT_3D_HPP
#include "../Math/Vector2.hpp"
#include "Window.hpp"
#include <vector>

#include <d3d11.h>

#pragma comment (lib, "d3d11.lib")

class Direct3D final
{
public:
	Direct3D();
	~Direct3D();

	bool Initialize(HWND__* windowHandle, const Vector2i& windowDimensions);

	void ClearDefaultTarget(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
	void ClearShadowTarget();
	void ClearAllTargets();

	void Present();

	void SetDefaultTarget();
	void SetShadowTarget();

	void SetDefaultBlendState();
	void SetDefaultRasterizerState();

	void EnableZBuffer();
	void DisableZBuffer();

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;

	ID3D11ShaderResourceView* GetShadowShaderResourceView();

private:
	/* Normal */
	bool InitializeDeviceAndSwapChain();
	bool InitializeDefaultRenderTargetView();
	bool InitializeDefaultDepthBufferAndDepthStencilView();
	bool InitializeDefaultViewport();
	bool InitializeDefaultBlendState();
	bool InitializeDefaultRasterizerState();

	/* Shadow */
	bool InitializeShadowDepthBufferAndDepthStencilView();
	bool InitializeShadowShaderResourceView();
	bool InitializeShadowViewport();

	/*
	Window
	*/
	HWND__* m_windowHandle;
	Vector2i m_windowDimensions;

	/*
	Core
	*/
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilState* m_depthDisabledStencilState;

	/*
	Normal rendering
	*/
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilView* m_depthStencilView;
	D3D11_VIEWPORT m_viewPort;

	ID3D11RasterizerState* m_defaultRasterizerState;

	/* Shadow */
	ID3D11ShaderResourceView* m_s_shaderResourceView;
	ID3D11Texture2D* m_s_depthStencilBuffer;
	ID3D11DepthStencilView* m_s_depthStencilView;
	D3D11_VIEWPORT m_s_viewport;

	
	ID3D11BlendState* m_defaultBlendState;
};

#endif