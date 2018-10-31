#include "Direct3D.hpp"
#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")

/*
Global viewPorts (could be allocated instead)
*/
D3D11_VIEWPORT viewPorts[2];

Direct3D::Direct3D()
{

}
Direct3D::~Direct3D()
{
	/*
	Release shadow pass resources
	*/
	if (m_sPassDSV)
	{
		m_sPassDSV->Release();
		m_sPassDSV = nullptr;
	}
	if (m_sPassDSB)
	{
		m_sPassDSB->Release();
		m_sPassDSB = nullptr;
	}
	if (m_sPassSRV)
	{
		m_sPassSRV->Release();
		m_sPassSRV = nullptr;
	}

	/*
	Release final pass resources
	*/
	if (m_fPassDSV)
	{
		m_fPassDSV->Release();
		m_fPassDSV = nullptr;
	}
	/*if (m_fPassDSB)
	{
		m_fPassDSB->Release();
		m_fPassDSB = nullptr;
	}*/
	if (m_fPassRTV)
	{
		m_fPassRTV->Release();
		m_fPassRTV = nullptr;
	}

	/*
	Release predefined states resources
	*/
	if (m_defaultBlendState)
	{
		m_defaultBlendState->Release();
		m_defaultBlendState = nullptr;
	}
	if (m_defaultRasterizerState)
	{
		m_defaultRasterizerState->Release();
		m_defaultRasterizerState = nullptr;
	}
	if (m_depthDisabledStencilState)
	{
		m_depthDisabledStencilState->Release();
		m_depthDisabledStencilState = nullptr;
	}
	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = nullptr;
	}

	/*
	Release core resources
	*/
	if (m_immediateDeviceContext)
	{
		m_immediateDeviceContext->Release();
		m_immediateDeviceContext = nullptr;
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
	if (!InitializeFinalRenderTargetView())
	{
		return false;
	}
	if (!InitializeFinalDepthBufferAndDepthStencilView())
	{
		return false;
	}
	if (!InitializeFinalViewport())
	{
		return false;
	}

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
	swapChainDesc.BufferCount = 1;												// one back buffer
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;	// use 32-bit color RGBA
	swapChainDesc.BufferDesc.Width = m_windowDimensions.x;
	swapChainDesc.BufferDesc.Height = m_windowDimensions.y;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;				// how swap chain is to be used
	swapChainDesc.OutputWindow = m_windowHandle;								// the window to be used
	swapChainDesc.SampleDesc.Count = 4;											// how many multisamples
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = true;												// windowed/full-screen mode

	result = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0U,
		nullptr,
		0U,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_swapChain,
		&m_device,
		nullptr,
		&m_immediateDeviceContext
		);

	if (FAILED(result))
	{
		return false;
	}
	return true;
}
bool Direct3D::InitializeFinalRenderTargetView()
{
	ID3D11Texture2D* backBuffer = nullptr;

	if (FAILED(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer)))
	{
		return false;
	}
	if (FAILED(m_device->CreateRenderTargetView(backBuffer, NULL, &m_fPassRTV)))
	{
		return false;
	}

	backBuffer->Release();
	backBuffer = nullptr;

	return true;
}
bool Direct3D::InitializeFinalDepthBufferAndDepthStencilView()
{
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	//D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	ID3D11Texture2D* fPassDSB = nullptr;
	//D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	HRESULT result;

	// Depth buffer ================================================================================================
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = m_windowDimensions.x;
	depthBufferDesc.Height = m_windowDimensions.y;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	depthBufferDesc.SampleDesc.Count = 4;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	depthBufferDesc.MiscFlags = 0;

	result = m_device->CreateTexture2D(&depthBufferDesc, nullptr, &fPassDSB);
	if (FAILED(result))
	{
		return false;
	}


	// Depth stencil state ================================================================================================
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
	
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	m_immediateDeviceContext->OMSetDepthStencilState(m_depthStencilState, 0);

	/*D3D11_FEATURE_DATA_THREADING featureSupport;
	m_device->CheckFeatureSupport(D3D11_FEATURE::D3D11_FEATURE_THREADING, &featureSupport, sizeof(D3D11_FEATURE_DATA_THREADING));*/

	// Depth disabled stencil state ================================================================================================
	/*ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;

	result = m_device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);
	if (FAILED(result))
	{
		return false;
	}*/
	depthStencilDesc.DepthEnable = false;
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthDisabledStencilState);
	if (FAILED(result))
	{
		return false;
	}


	// Depth stencil view ================================================================================================
	
	result = m_device->CreateDepthStencilView(fPassDSB, nullptr, &m_fPassDSV);
	if (FAILED(result))
	{
		return false;
	}

	fPassDSB->Release();

	m_immediateDeviceContext->OMSetRenderTargets(1, &m_fPassRTV, m_fPassDSV);

	return true;
}
bool Direct3D::InitializeFinalViewport()
{
	viewPorts[0].Width = (FLOAT)m_windowDimensions.x;
	viewPorts[0].Height = (FLOAT)m_windowDimensions.y;
	viewPorts[0].MinDepth = 0.0f;
	viewPorts[0].MaxDepth = 1.0f;
	viewPorts[0].TopLeftX = 0.0f;
	viewPorts[0].TopLeftY = 0.0f;
	m_fPassVP = &viewPorts[0];

	/*m_viewPort.Width = (FLOAT)m_windowDimensions.x;
	m_viewPort.Height = (FLOAT)m_windowDimensions.y;
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;
	m_viewPort.TopLeftX = 0.0f;
	m_viewPort.TopLeftY = 0.0f;*/

	m_immediateDeviceContext->RSSetViewports(1, m_fPassVP);

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
	desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
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

	if (FAILED(m_device->CreateTexture2D(&depthBufferDesc, nullptr, &m_sPassDSB)))
	{
		return false;
	}

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	if (FAILED(m_device->CreateDepthStencilView(m_sPassDSB, &depthStencilViewDesc, &m_sPassDSV)))
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

	if (FAILED(m_device->CreateShaderResourceView(m_sPassDSB, &shaderResourceViewDesc, &m_sPassSRV)))
	{
		return false;
	}

	return true;
}
bool Direct3D::InitializeShadowViewport()
{
	viewPorts[1].Width = (float)m_windowDimensions.x;
	viewPorts[1].Height = (float)m_windowDimensions.y;
	viewPorts[1].MinDepth = 0.0f;
	viewPorts[1].MaxDepth = 1.0f;
	viewPorts[1].TopLeftX = 0.0f;
	viewPorts[1].TopLeftY = 0.0f;

	m_sPassVP = &viewPorts[1];

	return true;
}

void Direct3D::ClearFinalTarget(float r, float g, float b, float a)
{
	float clearColor[] = { r, g, b, a };
	m_immediateDeviceContext->ClearRenderTargetView(m_fPassRTV, clearColor);
	m_immediateDeviceContext->ClearDepthStencilView(m_fPassDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
void Direct3D::ClearShadowTarget()
{
	m_immediateDeviceContext->ClearDepthStencilView(m_sPassDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Direct3D::Present()
{
	m_swapChain->Present(0, 0);
}

void Direct3D::SetShadowTarget()
{
	m_immediateDeviceContext->OMSetRenderTargets(0, nullptr, m_sPassDSV);
	m_immediateDeviceContext->RSSetViewports(1, m_sPassVP);
}
void Direct3D::SetFinalTarget()
{
	m_immediateDeviceContext->OMSetRenderTargets(1, &m_fPassRTV, m_fPassDSV);
	m_immediateDeviceContext->RSSetViewports(1, m_fPassVP);
}

void Direct3D::SetDefaultBlendState()
{
	const float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	const UINT sampleMask = 0xfffffff;
	
	m_immediateDeviceContext->OMSetBlendState(m_defaultBlendState, blendFactor, sampleMask);
}
void Direct3D::SetDefaultRasterizerState()
{
	m_immediateDeviceContext->RSSetState(m_defaultRasterizerState);
}

void Direct3D::EnableZBuffer()
{
	m_immediateDeviceContext->OMSetDepthStencilState(m_depthStencilState, 0);
}
void Direct3D::DisableZBuffer()
{
	m_immediateDeviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 0);
}

ID3D11Device* Direct3D::GetDevice() const
{
	return m_device;
}
ID3D11DeviceContext* Direct3D::GetDeviceContext() const
{
	return m_immediateDeviceContext;
}
ID3D11ShaderResourceView * Direct3D::GetShadowShaderResourceView() const
{
	return m_sPassSRV;
}