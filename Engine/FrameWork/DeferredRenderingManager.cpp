#include "DeferredRenderingManager.hpp"
#include "VertexBufferManager.hpp"
#include "ShaderManagerV2.hpp"
#include <d3d11.h>

DeferredRenderingManager::DeferredRenderingManager() :
	m_device(nullptr),
	m_deviceContext(nullptr),
	m_depthStencilView(nullptr),
	/*m_viewPort(nullptr),*/
	m_vertexBufferManager(nullptr),
	m_shaderManager(nullptr),
	m_vertexBufferID(-1)
{
	for (unsigned int i = 0; i < NR_OF_DEFERRED_OUTPUT_BUFFERS; i++)
	{
		m_renderTargetViews[i] = nullptr;
		m_shaderResourceViews[i] = nullptr;
	}
}

DeferredRenderingManager::~DeferredRenderingManager()
{
	/*if (m_viewPort)
	{
		delete m_viewPort;
	}*/

	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = nullptr;
	}
	for (unsigned int i = 0; i < NR_OF_DEFERRED_OUTPUT_BUFFERS; i++)
	{
		if (m_shaderResourceViews[i])
		{
			m_shaderResourceViews[i]->Release();
			m_shaderResourceViews[i] = nullptr;
		}
		if (m_renderTargetViews[i])
		{
			m_renderTargetViews[i]->Release();
			m_renderTargetViews[i] = nullptr;
		}
	}
}

bool DeferredRenderingManager::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, const Vector2i& windowDimensions, VertexBufferManager * vertexBufferManager, ShaderManagerV2 * shaderManager)
{
	m_device = device;
	m_deviceContext = deviceContext;
	m_shaderManager = shaderManager;
	m_vertexBufferManager = vertexBufferManager;

	/*
	Temporary texture buffers for creating rtv's, srv's and dsv's
	*/
	ID3D11Texture2D* renderTargetTextures[NR_OF_DEFERRED_OUTPUT_BUFFERS];
	ID3D11Texture2D* depthStencilBuffer;

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
	rttDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
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



#ifdef VARYING_TEXTURE_PIXEL_SIZES
	/*
	World positions require only 3 floats
	*/
	rttDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
	HRESULT result = m_device->CreateTexture2D(&rttDesc, nullptr, &renderTargetTextures[static_cast<unsigned int>(DeferredBufferTypes::WORLD_POSITIONS)]);
	if (FAILED(result))
		return false;
	/*
	Normals require only 3 floats
	*/
	rttDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	if (FAILED(m_device->CreateTexture2D(&rttDesc, nullptr, &renderTargetTextures[static_cast<unsigned int>(DeferredBufferTypes::NORMALS)])))
		return false;
	/*
	Colors require 4 floats
	*/
	rttDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
	if (FAILED(m_device->CreateTexture2D(&rttDesc, nullptr, &renderTargetTextures[static_cast<unsigned int>(DeferredBufferTypes::COLORS)])))
		return false;

	for (unsigned int i = 0; i < NR_OF_DEFERRED_OUTPUT_BUFFERS; i++)
	{
		if (FAILED(m_device->CreateRenderTargetView(renderTargetTextures[i], &rtvDesc, &m_renderTargetViews[i])))
			return false;
		if (FAILED(m_device->CreateShaderResourceView(renderTargetTextures[i], &srvDesc, &m_shaderResourceViews[i])))
			return false;
		renderTargetTextures[i]->Release();
	}

#else
	for (unsigned int i = 0; i < NR_OF_DEFERRED_OUTPUT_BUFFERS; i++)
	{
		/*
		Create texture buffers
		*/
		if (FAILED(m_device->CreateTexture2D(&rttDesc, nullptr, &renderTargetTextures[i])))
			return false;
		/*
		Create render targets
		*/
		if (FAILED(m_device->CreateRenderTargetView(renderTargetTextures[i], &rtvDesc, &m_renderTargetViews[i])))
			return false;
		/*
		Create shader resources
		*/
		if (FAILED(m_device->CreateShaderResourceView(renderTargetTextures[i], &srvDesc, &m_shaderResourceViews[i])))
			return false;
	}
#endif



	/*
	Create depth stencil buffer
	*/
	if (FAILED(m_device->CreateTexture2D(&dbDesc, nullptr, &depthStencilBuffer)))
		return false;
	/*
	Create depth stencil view
	*/
	if (FAILED(m_device->CreateDepthStencilView(depthStencilBuffer, &dsvDesc, &m_depthStencilView)))
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
	Create vertex buffer for screen target
	*/

	/* A triangle perfectly covering the screen */
	m_vertices[0].position = Vector3f(-3.0f, -1.0f, 0.0f);	// bottom left
	m_vertices[1].position = Vector3f(1.0f, 3.0f, 0.0f);	// upper right
	m_vertices[2].position = Vector3f(1.0f, -1.0f, 0.0f);	// bottom right
	m_vertices[0].uv = Vector2f(-1.0f, 1.0f);
	m_vertices[1].uv = Vector2f(1.0f, -1.0f);
	m_vertices[2].uv = Vector2f(1.0f, 1.0f);

	/* A triangle covering half the screen */
	//m_vertices[0].position = Vector3f(-1.0f, -1.0f, 0.0f);	// bottom left
	//m_vertices[1].position = Vector3f(1.0f, 1.0f, 0.0f);	// upper right
	//m_vertices[2].position = Vector3f(1.0f, -1.0f, 0.0f);	// bottom right
	//m_vertices[0].uv = Vector2f(0.0f, 1.0f);
	//m_vertices[1].uv = Vector2f(1.0f, 0.0f);
	//m_vertices[2].uv = Vector2f(1.0f, 1.0f);


	m_vertexBufferID = m_vertexBufferManager->CreateBuffer(sizeof(Vertex) * 3, sizeof(Vertex), m_vertices);
	if (m_vertexBufferID == -1)
		return false;

	return true;
}

void DeferredRenderingManager::RenderLightPass()
{
	m_shaderManager->SetShaders(ShaderTypeV2::LIGHT_PASS);

	// Setting light pass render target is done before this

	m_vertexBufferManager->SetBufferToInputAssembler(m_vertexBufferID, 0);
	
	/*
	This and Direct3D should be changed to store the deferred textures in the texture manager instead,
	then utilizing it when setting a texture to the shader
	*/
	m_deviceContext->PSSetShaderResources(0, NR_OF_DEFERRED_OUTPUT_BUFFERS, m_shaderResourceViews);
	m_deviceContext->Draw(sizeof(m_vertices) / sizeof(Vertex), 0);
}

void DeferredRenderingManager::ClearRenderTargets(float r, float g, float b, float a)
{
	float clearColor[] = { r, g, b, a };
	for (unsigned int i = 0; i < NR_OF_DEFERRED_OUTPUT_BUFFERS; i++)
	{
		m_deviceContext->ClearRenderTargetView(m_renderTargetViews[i], clearColor);
	}

	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DeferredRenderingManager::SetRenderTargets()
{
	m_deviceContext->OMSetRenderTargets(NR_OF_DEFERRED_OUTPUT_BUFFERS, m_renderTargetViews, m_depthStencilView);
	//m_deviceContext->RSSetViewports(1, m_viewPort);
}

ID3D11ShaderResourceView ** DeferredRenderingManager::GetShaderResourceViews()
{
	return m_shaderResourceViews;
}
