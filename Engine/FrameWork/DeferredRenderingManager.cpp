#include "DeferredRenderingManager.hpp"
#include <d3d11.h>

DeferredRenderingManager::DeferredRenderingManager() :
	m_device(nullptr), m_deviceContext(nullptr), m_gPassDSV(nullptr),
	m_lPassRTV(nullptr), m_lPassSRV(nullptr), m_lPassDSV(nullptr)
{
	for (unsigned int i = 0; i < NR_OF_DEFERRED_OUTPUT_BUFFERS; i++)
	{
		m_gPassRTVs[i] = nullptr;
		m_gPassSRVs[i] = nullptr;
	}
}

DeferredRenderingManager::~DeferredRenderingManager()
{
	/*
	Release light pass resources
	*/
	if (m_lPassDSV)
	{
		m_lPassDSV->Release();
		m_lPassDSV = nullptr;
	}
	if (m_lPassSRV)
	{
		m_lPassSRV->Release();
		m_lPassSRV = nullptr;
	}
	if (m_lPassRTV)
	{
		m_lPassRTV->Release();
		m_lPassRTV = nullptr;
	}

	/*
	Release geometry pass resources
	*/
	if (m_gPassDSV)
	{
		m_gPassDSV->Release();
		m_gPassDSV = nullptr;
	}
	for (unsigned int i = 0; i < NR_OF_DEFERRED_OUTPUT_BUFFERS; i++)
	{
		if (m_gPassSRVs[i])
		{
			m_gPassSRVs[i]->Release();
			m_gPassSRVs[i] = nullptr;
		}
		if (m_gPassRTVs[i])
		{
			m_gPassRTVs[i]->Release();
			m_gPassRTVs[i] = nullptr;
		}
	}
}

bool DeferredRenderingManager::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, const Vector2i& windowDimensions)
{
	m_device = device;
	m_deviceContext = deviceContext;

	/*
	Temporary texture buffers for creating rtv's, srv's and dsv's
	*/
	ID3D11Texture2D* renderTargetTextures[NR_OF_DEFERRED_OUTPUT_BUFFERS];
	ID3D11Texture2D* depthStencilBuffer;

	ID3D11Texture2D* lightRenderTargetTexture;
	ID3D11Texture2D* lightDepthStencilBuffer;

	/*
	Descriptions
	*/
	D3D11_TEXTURE2D_DESC rttDesc;
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	D3D11_TEXTURE2D_DESC dbDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;

	ZeroMemory(&rttDesc, sizeof(rttDesc));
	rttDesc.Width = windowDimensions.x;
	rttDesc.Height = windowDimensions.y;
	rttDesc.MipLevels = 1;
	rttDesc.ArraySize = 1;
	rttDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT;
	rttDesc.SampleDesc.Count = 1;
	rttDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	rttDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
	rttDesc.CPUAccessFlags = 0;
	rttDesc.MiscFlags = 0;

	ZeroMemory(&rtvDesc, sizeof(rtvDesc));
	rtvDesc.Format = rttDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION::D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = rttDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	// Previously used / tested formats
	/*DXGI_FORMAT_D32_FLOAT*/ /*DXGI_FORMAT_R32G32B32A32_FLOAT*//*DXGI_FORMAT_D24_UNORM_S8_UINT*/
	ZeroMemory(&dbDesc, sizeof(dbDesc));
	dbDesc.Width = windowDimensions.x;
	dbDesc.Height = windowDimensions.y;
	dbDesc.MipLevels = 1;
	dbDesc.ArraySize = 1;
	dbDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS;
	dbDesc.SampleDesc.Count = 1;
	dbDesc.SampleDesc.Quality = 0;
	dbDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	dbDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
	dbDesc.CPUAccessFlags = /*D3D11_USAGE_DEFAULT*/ 0;
	dbDesc.MiscFlags = 0;

	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	for (unsigned int i = 0; i < NR_OF_DEFERRED_OUTPUT_BUFFERS; i++)
	{
		if (FAILED(m_device->CreateTexture2D(&rttDesc, nullptr, &renderTargetTextures[i])))
			return false;
		if (FAILED(m_device->CreateRenderTargetView(renderTargetTextures[i], &rtvDesc, &m_gPassRTVs[i])))
			return false;
		if (FAILED(m_device->CreateShaderResourceView(renderTargetTextures[i], &srvDesc, &m_gPassSRVs[i])))
			return false;
		renderTargetTextures[i]->Release();
	}

	if (FAILED(m_device->CreateTexture2D(&dbDesc, nullptr, &depthStencilBuffer)))
		return false;
	if (FAILED(m_device->CreateDepthStencilView(depthStencilBuffer, &dsvDesc, &m_gPassDSV)))
		return false;
	depthStencilBuffer->Release();

	/*m_viewPort = new D3D11_VIEWPORT;
	m_viewPort->Width = static_cast<float>(windowDimensions.x);
	m_viewPort->Height = static_cast<float>(windowDimensions.y);
	m_viewPort->MinDepth = 0.0f;
	m_viewPort->MaxDepth = 1.0f;
	m_viewPort->TopLeftX = 0.0f;
	m_viewPort->TopLeftY = 0.0f;*/

	/*
	Create resources for light pass
	*/
	if (FAILED(m_device->CreateTexture2D(&rttDesc, nullptr, &lightRenderTargetTexture)))
		return false;
	if (FAILED(m_device->CreateRenderTargetView(lightRenderTargetTexture, &rtvDesc, &m_lPassRTV)))
		return false;
	if (FAILED(m_device->CreateShaderResourceView(lightRenderTargetTexture, &srvDesc, &m_lPassSRV)))
		return false;
	if (FAILED(m_device->CreateTexture2D(&dbDesc, nullptr, &lightDepthStencilBuffer)))
		return false;
	if (FAILED(m_device->CreateDepthStencilView(lightDepthStencilBuffer, &dsvDesc, &m_lPassDSV)))
		return false;

	return true;
}

void DeferredRenderingManager::RenderLightPass()
{
	/*
	This and Direct3D should be changed to store the deferred textures in the texture manager instead,
	then utilizing it when setting a texture to the shader
	*/
	m_deviceContext->PSSetShaderResources(0, NR_OF_DEFERRED_OUTPUT_BUFFERS, m_gPassSRVs);
	m_deviceContext->Draw(3, 0);
}

void DeferredRenderingManager::ClearGeometryRenderTargets(float r, float g, float b, float a)
{
	float clearColor[] = { r, g, b, a };
	for (unsigned int i = 0; i < NR_OF_DEFERRED_OUTPUT_BUFFERS; i++)
	{
		m_deviceContext->ClearRenderTargetView(m_gPassRTVs[i], clearColor);
	}

	m_deviceContext->ClearDepthStencilView(m_gPassDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
void DeferredRenderingManager::ClearLightRenderTarget(float r, float g, float b, float a)
{
	float clearColor[] = { r, g, b, a };

	m_deviceContext->ClearRenderTargetView(m_lPassRTV, clearColor);
	m_deviceContext->ClearDepthStencilView(m_lPassDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DeferredRenderingManager::SetGeometryRenderTargets()
{
	m_deviceContext->OMSetRenderTargets(NR_OF_DEFERRED_OUTPUT_BUFFERS, m_gPassRTVs, m_gPassDSV);
}
void DeferredRenderingManager::SetLightRenderTarget()
{
	m_deviceContext->OMSetRenderTargets(1, &m_lPassRTV, m_lPassDSV);
}

ID3D11ShaderResourceView ** DeferredRenderingManager::GetGeometryPassShaderResourceViews()
{
	return m_gPassSRVs;
}
ID3D11ShaderResourceView * DeferredRenderingManager::GetLightPassShaderResourceView()
{
	return m_lPassSRV;
}