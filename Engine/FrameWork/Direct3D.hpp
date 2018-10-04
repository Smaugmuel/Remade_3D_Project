#ifndef DIRECT_3D_HPP
#define DIRECT_3D_HPP
#include "../Math/Vector2.hpp"
#include "Window.hpp"

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;
struct ID3D11DepthStencilView;
struct ID3D11DepthStencilState;
struct ID3D11RasterizerState;
struct ID3D11BlendState;
struct ID3D11Texture2D;
struct D3D11_VIEWPORT;

class Direct3D final
{
public:
	Direct3D();
	~Direct3D();

	bool Initialize(HWND__* windowHandle, const Vector2i& windowDimensions);

	void ClearFinalTarget(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
	void ClearShadowTarget();

	void Present();

	void SetFinalTarget();
	void SetShadowTarget();

	void SetDefaultBlendState();
	void SetDefaultRasterizerState();

	void EnableZBuffer();
	void DisableZBuffer();

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;
	ID3D11ShaderResourceView* GetShadowShaderResourceView() const;

private:
	/* Normal */
	bool InitializeDeviceAndSwapChain();
	bool InitializeFinalRenderTargetView();
	bool InitializeFinalDepthBufferAndDepthStencilView();
	bool InitializeFinalViewport();
	bool InitializeDefaultBlendState();
	bool InitializeDefaultRasterizerState();

	/* Shadow */
	bool InitializeShadowDepthBufferAndDepthStencilView();
	bool InitializeShadowShaderResourceView();
	bool InitializeShadowViewport();

	/*
	Window resources
	*/
	HWND__* m_windowHandle;
	Vector2i m_windowDimensions;

	/*
	Core resources
	*/
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_immediateDeviceContext;


	/*
	Predefined states
	*/
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilState* m_depthDisabledStencilState;
	ID3D11RasterizerState* m_defaultRasterizerState;
	ID3D11BlendState* m_defaultBlendState;

	/*
	Final pass resources
	*/
	ID3D11RenderTargetView* m_fPassRTV;
	ID3D11DepthStencilView* m_fPassDSV;
	D3D11_VIEWPORT* m_fPassVP;

	/*
	Shadow pass resources
	*/
	ID3D11ShaderResourceView* m_sPassSRV;
	ID3D11Texture2D* m_sPassDSB;
	ID3D11DepthStencilView* m_sPassDSV;
	D3D11_VIEWPORT* m_sPassVP;
};

#endif