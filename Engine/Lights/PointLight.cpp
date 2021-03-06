#include "PointLight.hpp"
#include "../Camera/Camera.hpp"
#include <d3d11.h>

PointLight::PointLight()
{
	m_viewport = nullptr;
}

PointLight::~PointLight()
{
	if (m_viewport)
	{
		delete m_viewport;
	}
	if (m_camera)
	{
		delete m_camera;
	}
}

bool PointLight::Initialize(ID3D11Device * device, Vector2f dimensions)
{
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	depthBufferDesc.Width = static_cast<unsigned int>(dimensions.x);
	depthBufferDesc.Height = static_cast<unsigned int>(dimensions.y);	// Change to quadratic later ?
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	if (FAILED(device->CreateTexture2D(&depthBufferDesc, nullptr, &m_depthStencilBuffer)))
	{
		return false;
	}

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	if (FAILED(device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView)))
	{
		return false;
	}

	shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	if (FAILED(device->CreateShaderResourceView(m_depthStencilBuffer, &shaderResourceViewDesc, &m_resource)))
	{
		return false;
	}

	m_viewport = new D3D11_VIEWPORT;
	m_viewport->Width = dimensions.x;
	m_viewport->Height = dimensions.y;
	m_viewport->MinDepth = 0.0f;
	m_viewport->MaxDepth = 1.0f;
	m_viewport->TopLeftX = 0.0f;
	m_viewport->TopLeftY = 0.0f;


	m_camera = new Camera;
	m_camera->Initialize();

	return true;
}

void PointLight::Update()
{
	m_camera->Update();
}

void PointLight::Clear(ID3D11DeviceContext * deviceContext)
{
	deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void PointLight::Set(ID3D11DeviceContext * deviceContext)
{
	deviceContext->OMSetRenderTargets(0, nullptr, m_depthStencilView);
	deviceContext->RSSetViewports(1, m_viewport);
}

void PointLight::SetPosition(Vector3f position)
{
	m_camera->SetPosition(position);
}

void PointLight::SetTarget(Vector3f target)
{
	m_camera->SetTarget(target);
}

Vector3f PointLight::GetPosition() const
{
	return m_camera->GetPosition();
}

Vector3f PointLight::GetTarget() const
{
	return m_camera->GetTarget();
}

DirectX::XMMATRIX PointLight::GetView() const
{
	return m_camera->GetViewMatrix();
}

DirectX::XMMATRIX PointLight::GetProjection() const
{
	return m_camera->GetProjectionMatrix();
}

ID3D11ShaderResourceView * PointLight::GetResource()
{
	return m_resource;
}
