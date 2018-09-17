#include "ShadowMapRenderingManager.hpp"
#include <d3d11.h>

D3D11_VIEWPORT gViewPort;

ShadowMapRenderingManager::ShadowMapRenderingManager() :
	m_device(nullptr), m_deviceContext(nullptr),
	m_depthStencilBuffer(nullptr), m_depthStencilView(nullptr),
	m_shaderResourceView(nullptr)
{
}

ShadowMapRenderingManager::~ShadowMapRenderingManager()
{
}

bool ShadowMapRenderingManager::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, const Vector2i& windowDimensions)
{
	m_device = device;
	m_deviceContext = deviceContext;

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	depthBufferDesc.Width = windowDimensions.x;
	depthBufferDesc.Height = windowDimensions.y;	// Change to quadratic later ?
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	if (FAILED(m_device->CreateTexture2D(&depthBufferDesc, nullptr, &m_depthStencilBuffer)))
		return false;

	if (FAILED(m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView)))
		return false;

	if (FAILED(m_device->CreateShaderResourceView(m_depthStencilBuffer, &shaderResourceViewDesc, &m_shaderResourceView)))
		return false;

	gViewPort.Width = (float)windowDimensions.x;
	gViewPort.Height = (float)windowDimensions.y;
	gViewPort.MinDepth = 0.0f;
	gViewPort.MaxDepth = 1.0f;
	gViewPort.TopLeftX = 0.0f;
	gViewPort.TopLeftY = 0.0f;

	return true;
}

void ShadowMapRenderingManager::ClearDepthStencilView()
{
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void ShadowMapRenderingManager::SetRenderTarget()
{
	m_deviceContext->OMSetRenderTargets(0, nullptr, m_depthStencilView);
	m_deviceContext->RSSetViewports(1, &gViewPort);
}

ID3D11ShaderResourceView * ShadowMapRenderingManager::GetShaderResourceView()
{
	return m_shaderResourceView;
}
