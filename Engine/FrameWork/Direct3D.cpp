#include "Direct3D.hpp"

Direct3D::Direct3D()
{

}
Direct3D::~Direct3D()
{
	if (m_defaultBlendState)
	{
		m_defaultBlendState->Release();
		m_defaultBlendState = nullptr;
	}

	/* Shadow */
	if (m_s_shaderResourceView)
	{
		m_s_shaderResourceView->Release();
		m_s_shaderResourceView = nullptr;
	}
	if (m_s_depthStencilView)
	{
		m_s_depthStencilView->Release();
		m_s_depthStencilView = nullptr;
	}
	if (m_s_depthStencilBuffer)
	{
		m_s_depthStencilBuffer->Release();
		m_s_depthStencilBuffer = nullptr;
	}


	/* Render to texture */
	//if (m_rt_shaderResourceView)
	//{
	//	m_rt_shaderResourceView->Release();
	//	m_rt_shaderResourceView = nullptr;
	//}
	//if (m_rt_renderTargetView)
	//{
	//	m_rt_renderTargetView->Release();
	//	m_rt_renderTargetView = nullptr;
	//}
	//if (m_rt_renderTargetTexture)
	//{
	//	m_rt_renderTargetTexture->Release();
	//	m_rt_renderTargetTexture = nullptr;
	//}


	// Default
	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = nullptr;
	}

	/* Added with rastertek */
	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = nullptr;
	}
	if (m_depthDisabledStencilState)
	{
		m_depthDisabledStencilState->Release();
		m_depthDisabledStencilState = nullptr;
	}


	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = nullptr;
	}
	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = nullptr;
	}
	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = nullptr;
	}
	if (m_device)
	{
		m_device->Release();
		m_device = nullptr;
	}
	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = nullptr;
	}
}

bool Direct3D::Initialize(HWND__* windowHandle, const Vector2i& windowDimensions)
{
	if (windowDimensions.x <= 0 || windowDimensions.y <= 0)
	{
		return false;
	}

	m_windowHandle = windowHandle;
	m_windowDimensions = windowDimensions;

	if (!InitializeDeviceAndSwapChain())
	{
		return false;
	}
	if (!InitializeDefaultRenderTargetView())
	{
		return false;
	}
	if (!InitializeDefaultDepthBufferAndDepthStencilView())
	{
		return false;
	}
	if (!InitializeDefaultViewport())
	{
		return false;
	}

	// Deferred
	/*if (!m_deferredRenderingManager.InitializeNormal(m_device, m_deviceContext, windowDimensions))
	{
		return false;
	}*/

	/* Render to texture */
	//if (!InitializeRenderToTextureRenderTargetView())
	//{
	//	return false;
	//}

	/* Shadow */
	if (!InitializeShadowDepthBufferAndDepthStencilView())
	{
		return false;
	}
	if (!InitializeShadowShaderResourceView())
	{
		return false;
	}
	if (!InitializeShadowViewport())
	{
		return false;
	}

	return true;
}

bool Direct3D::InitializeDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	HRESULT result;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferCount = 1;									// one back buffer
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// use 32-bit color RGBA
	swapChainDesc.BufferDesc.Width = m_windowDimensions.x;
	swapChainDesc.BufferDesc.Height = m_windowDimensions.y;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// how swap chain is to be used
	swapChainDesc.OutputWindow = m_windowHandle;					// the window to be used
	swapChainDesc.SampleDesc.Count = 4;								// how many multisamples
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;									// windowed/full-screen mode

	result = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_swapChain,
		&m_device,
		NULL,
		&m_deviceContext
		);

	if (FAILED(result))
	{
		return false;
	}
	return true;
}
bool Direct3D::InitializeDefaultRenderTargetView()
{
	ID3D11Texture2D* backBuffer = nullptr;

	if (FAILED(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer)))
	{
		return false;
	}
	if (FAILED(m_device->CreateRenderTargetView(backBuffer, NULL, &m_renderTargetView)))
	{
		return false;
	}

	backBuffer->Release();
	backBuffer = nullptr;

	return true;
}
bool Direct3D::InitializeDefaultDepthBufferAndDepthStencilView()
{
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	//D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	HRESULT result;

	// Depth buffer ================================================================================================
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = m_windowDimensions.x;
	depthBufferDesc.Height = m_windowDimensions.y;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthBufferDesc.SampleDesc.Count = 4;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = D3D11_USAGE_DEFAULT;
	depthBufferDesc.MiscFlags = 0;

	result = m_device->CreateTexture2D(&depthBufferDesc, nullptr, &m_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}


	// Depth stencil state ================================================================================================
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 0);


	// Depth disabled stencil state ================================================================================================
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = m_device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);
	if (FAILED(result))
	{
		return false;
	}


	// Depth stencil view ================================================================================================
	
	//ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	//depthStencilViewDesc.Flags = 0;
	//depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	//depthStencilViewDesc.Texture2D.MipSlice = 0;

	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, nullptr, &m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	return true;
}
bool Direct3D::InitializeDefaultViewport()
{
	m_viewPort.Width = (FLOAT)m_windowDimensions.x;
	m_viewPort.Height = (FLOAT)m_windowDimensions.y;
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;
	m_viewPort.TopLeftX = 0.0f;
	m_viewPort.TopLeftY = 0.0f;

	m_deviceContext->RSSetViewports(1, &m_viewPort);

	return true;
}

bool Direct3D::InitializeDefaultBlendState()
{
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
	blendDesc.AlphaToCoverageEnable = true;
	blendDesc.IndependentBlendEnable = true;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT result = m_device->CreateBlendState(&blendDesc, &m_defaultBlendState);
	if (FAILED(result))
	{
		return false;
	}

	SetDefaultBlendState();

	return true;
}
bool Direct3D::InitializeDefaultRasterizerState()
{
	D3D11_RASTERIZER_DESC desc;
	desc.AntialiasedLineEnable = false;
	desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	desc.DepthBias = 0;
	desc.DepthBiasClamp = 0.0f;
	desc.DepthClipEnable = true;
	desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	desc.FrontCounterClockwise = true;
	desc.MultisampleEnable = false;
	desc.ScissorEnable = false;
	desc.SlopeScaledDepthBias = 0.0f;

	if (FAILED(m_device->CreateRasterizerState(&desc, &m_defaultRasterizerState)))
	{
		return false;
	}

	return true;
}

bool Direct3D::InitializeShadowDepthBufferAndDepthStencilView()
{
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	depthBufferDesc.Width = m_windowDimensions.x;
	depthBufferDesc.Height = m_windowDimensions.y;	// Change to quadratic later ?
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	if (FAILED(m_device->CreateTexture2D(&depthBufferDesc, nullptr, &m_s_depthStencilBuffer)))
	{
		return false;
	}

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	if (FAILED(m_device->CreateDepthStencilView(m_s_depthStencilBuffer, &depthStencilViewDesc, &m_s_depthStencilView)))
	{
		return false;
	}

	return true;
}
bool Direct3D::InitializeShadowShaderResourceView()
{
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	if (FAILED(m_device->CreateShaderResourceView(m_s_depthStencilBuffer, &shaderResourceViewDesc, &m_s_shaderResourceView)))
	{
		return false;
	}

	return true;
}
bool Direct3D::InitializeShadowViewport()
{
	m_s_viewport.Width = (float)m_windowDimensions.x;
	m_s_viewport.Height = (float)m_windowDimensions.y;
	m_s_viewport.MinDepth = 0.0f;
	m_s_viewport.MaxDepth = 1.0f;
	m_s_viewport.TopLeftX = 0.0f;
	m_s_viewport.TopLeftY = 0.0f;

	return true;
}

void Direct3D::ClearDefaultTarget(float r, float g, float b, float a)
{
	float clearColor[] = { r, g, b, a };
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, clearColor);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
//void Direct3D::ClearDeferredTargets(float r, float g, float b, float a)
//{
//	m_deferredRenderingManager.ClearRenderTargets(r, g, b, a);
//}
void Direct3D::ClearShadowTarget()
{
	m_deviceContext->ClearDepthStencilView(m_s_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
void Direct3D::ClearAllTargets()
{
	ClearDefaultTarget();
	//ClearDeferredTargets();
	ClearShadowTarget();
}

void Direct3D::Present()
{
	m_swapChain->Present(0, 0);
}

void Direct3D::SetDefaultTarget()
{
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
	m_deviceContext->RSSetViewports(1, &m_viewPort);
}
//void Direct3D::SetDeferredTargets()
//{
//	m_deferredRenderingManager.SetRenderTargets();
//}
void Direct3D::SetShadowTarget()
{
	m_deviceContext->OMSetRenderTargets(0, nullptr, m_s_depthStencilView);
	m_deviceContext->RSSetViewports(1, &m_s_viewport);
}

void Direct3D::SetDefaultBlendState()
{
	const float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	const UINT sampleMask = 0xfffffff;
	
	m_deviceContext->OMSetBlendState(m_defaultBlendState, blendFactor, sampleMask);
}

void Direct3D::SetDefaultRasterizerState()
{
	m_deviceContext->RSSetState(m_defaultRasterizerState);
}

void Direct3D::EnableZBuffer()
{
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 0);
}
void Direct3D::DisableZBuffer()
{
	m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 0);
}

ID3D11Device* Direct3D::GetDevice() const
{
	return m_device;
}
ID3D11DeviceContext* Direct3D::GetDeviceContext() const
{
	return m_deviceContext;
}

//ID3D11ShaderResourceView ** Direct3D::GetDeferredShaderResourceViews()
//{
//	return m_deferredRenderingManager.GetShaderResourceViews();
//}
ID3D11ShaderResourceView * Direct3D::GetShadowShaderResourceView()
{
	return m_s_shaderResourceView;
}