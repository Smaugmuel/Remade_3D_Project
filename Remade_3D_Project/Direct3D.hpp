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
	D_WORLD_POS,
	D_NORMALS,
	D_COLOR,
	NR_OF_ELEMENTS
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
	void ClearAllTargets();
	void Present();

	void SetDefaultTarget();
	void SetDeferredTargets();

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;

	ID3D11ShaderResourceView** GetShaderResourceViews();

private:
	bool InitializeDeviceAndSwapChain();
	bool InitializeDefaultRenderTargetView();
	bool InitializeDefaultDepthBufferAndDepthStencilView();
	bool InitializeDefaultViewport();

	// Deferred
	bool InitializeDeferredRenderTargetViews();
	bool InitializeDeferredDepthBufferAndDepthStencilView();
	bool InitializeDeferredViewport();

	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilView* m_depthStencilView;
	D3D11_VIEWPORT m_viewPort;

	// Deferred rendering
	ID3D11Texture2D* m_d_renderTargetTextures[BufferType::NR_OF_ELEMENTS];
	ID3D11RenderTargetView* m_d_renderTargetViews[BufferType::NR_OF_ELEMENTS];
	ID3D11ShaderResourceView* m_d_shaderResourceViews[BufferType::NR_OF_ELEMENTS];
	ID3D11Texture2D* m_d_depthStencilBuffer;
	ID3D11DepthStencilView* m_d_depthStencilView;
	D3D11_VIEWPORT m_d_viewPort;


	HWND m_windowHandle;
	Vector2i m_windowDimensions;
};

#endif