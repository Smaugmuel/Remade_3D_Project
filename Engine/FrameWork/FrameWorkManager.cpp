#include "FrameWorkManager.hpp"
#include <d3d11.h>

FrameWorkManager::FrameWorkManager()
{
}

FrameWorkManager::~FrameWorkManager()
{
}

#ifdef USES_MATRIX_BUFFER
bool FrameWorkManager::Initialize(Vector2i windowSize, int maxNrOfLights, int nrOfWorldMatricesInBuffer)
#else
bool FrameWorkManager::Initialize(Vector2i windowSize, int maxNrOfLights)
#endif
{
	if (!m_window.Initialize(windowSize))
		return false;
	if (!m_input.Initialize(m_window.GetHandle()))
		return false;
	if (!m_d3d.Initialize(m_window.GetHandle(), m_window.GetDimensions()))
		return false;
	if (!m_constantBufferManager.Initialize(m_d3d.GetDevice(), m_d3d.GetDeviceContext()))
		return false;
	if (!m_vertexBufferManager.Initialize(m_d3d.GetDevice(), m_d3d.GetDeviceContext()))
		return false;
#ifdef USES_MATRIX_BUFFER
	if (!m_shaderManager.Initialize(m_d3d.GetDevice(), m_d3d.GetDeviceContext(), maxNrOfLights, nrOfWorldMatricesInBuffer))
#else
	if (!m_shaderManager.Initialize(m_d3d.GetDevice(), m_d3d.GetDeviceContext(), maxNrOfLights))
#endif
		return false;
	if (!m_textureManager.Initialize(m_d3d.GetDevice(), m_d3d.GetDeviceContext()))
		return false;
	if (!m_guiManager.Initialize(&m_d3d, &m_textureManager))
		return false;
	if (!m_samplerManager.Initialize(m_d3d.GetDevice(), m_d3d.GetDeviceContext()))
		return false;
	if (!m_deferredRenderingManager.Initialize(m_d3d.GetDevice(), m_d3d.GetDeviceContext(), m_window.GetDimensions()))
		return false;
	if (!m_shadowMapRenderingManager.Initialize(m_d3d.GetDevice(), m_d3d.GetDeviceContext(), m_window.GetDimensions()))
		return false;
	if (!m_onScreenTarget.Initialize(&m_vertexBufferManager))
		return false;

	return true;
}

void FrameWorkManager::SetGeometryPassRenderTargets()
{
	m_deferredRenderingManager.SetGeometryRenderTargets();
}

void FrameWorkManager::SetLightPassRenderTarget()
{
	m_onScreenTarget.SetAsTarget();
	m_deferredRenderingManager.SetLightRenderTarget();
}

void FrameWorkManager::SetShadowPassRenderTarget()
{
	m_onScreenTarget.SetAsTarget();
	m_shadowMapRenderingManager.SetRenderTarget();
}

void FrameWorkManager::SetFinalPassRenderTarget()
{
	m_onScreenTarget.SetAsTarget();
	m_d3d.SetFinalTarget();
}

void FrameWorkManager::ClearGeometryPassRenderTargets(float r, float g, float b, float a)
{
	m_deferredRenderingManager.ClearGeometryRenderTargets(r, g, b, a);
}

void FrameWorkManager::ClearLightPassRenderTargets(float r, float g, float b, float a)
{
	m_deferredRenderingManager.ClearLightRenderTarget(r, g, b, a);
}

void FrameWorkManager::ClearShadowPassRenderTargets()
{
	m_shadowMapRenderingManager.ClearDepthStencilView();
}

void FrameWorkManager::ClearFinalPassRenderTarget(float r, float g, float b, float a)
{
	m_d3d.ClearFinalTarget(r, g, b, a);
}

void FrameWorkManager::RenderGeometryPassWithCurrentSettings(int nrOfVertices)
{
	ID3D11DeviceContext* deviceContext = m_d3d.GetDeviceContext();
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->Draw(nrOfVertices, 0);
}

void FrameWorkManager::RenderLightPass()
{
	m_deferredRenderingManager.SetLightRenderTarget();
	m_deferredRenderingManager.RenderLightPass();
}

void FrameWorkManager::RenderFinalPass()
{
	ID3D11ShaderResourceView* srv = m_deferredRenderingManager.GetLightPassShaderResourceView();
	ID3D11DeviceContext* deviceContext = m_d3d.GetDeviceContext();
	deviceContext->PSSetShaderResources(0, 1, &srv);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->Draw(3, 0);
}

void FrameWorkManager::Present()
{
	m_d3d.Present();
}

Window * FrameWorkManager::GetWindow()
{
	return &m_window;
}

Input * FrameWorkManager::GetInput()
{
	return &m_input;
}

Direct3D * FrameWorkManager::GetDirect3D()
{
	return &m_d3d;
}

ConstantBufferManager * FrameWorkManager::GetConstantBufferManager()
{
	return &m_constantBufferManager;
}

VertexBufferManager * FrameWorkManager::GetVertexBufferManager()
{
	return &m_vertexBufferManager;
}

ShaderManagerV2 * FrameWorkManager::GetShaderManager()
{
	return &m_shaderManager;
}

TextureManager * FrameWorkManager::GetTextureManager()
{
	return &m_textureManager;
}

GUIManager * FrameWorkManager::GetGUIManager()
{
	return &m_guiManager;
}

SamplerManager * FrameWorkManager::GetSamplerManager()
{
	return &m_samplerManager;
}
