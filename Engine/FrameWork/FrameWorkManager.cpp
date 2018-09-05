#include "FrameWorkManager.hpp"

FrameWorkManager::FrameWorkManager()
{
}

FrameWorkManager::~FrameWorkManager()
{
}

bool FrameWorkManager::Initialize(Vector2i windowSize)
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
	if (!m_deferredRenderingManager.Initialize(m_d3d.GetDevice(), m_d3d.GetDeviceContext(), m_window.GetDimensions(), &m_vertexBufferManager, &m_shaderManager))
		return false;
	if (!m_shaderManager.Initialize(m_d3d.GetDevice(), m_d3d.GetDeviceContext()))
		return false;
	if (!m_textureManager.Initialize(m_d3d.GetDevice(), m_d3d.GetDeviceContext()))
		return false;
	if (!m_guiManager.Initialize(&m_d3d, &m_textureManager))
		return false;
	if (!m_samplerManager.Initialize(m_d3d.GetDevice(), m_d3d.GetDeviceContext()))
		return false;

	return true;
}

void FrameWorkManager::SetFirstPassRenderTargets()
{
	m_deferredRenderingManager.SetRenderTargets();
}

void FrameWorkManager::SetLightPassRenderTarget()
{
	m_d3d.SetDefaultTarget();
}

void FrameWorkManager::ClearFirstPassRenderTargets(float r, float g, float b, float a)
{
	m_deferredRenderingManager.ClearRenderTargets(r, g, b, a);
}

void FrameWorkManager::ClearLightPassRenderTargets(float r, float g, float b, float a)
{
	m_d3d.ClearDefaultTarget(r, g, b, a);
}

void FrameWorkManager::RenderWithCurrentSettings(int nrOfVertices)
{
	m_d3d.GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_d3d.GetDeviceContext()->Draw(nrOfVertices, 0);
}

void FrameWorkManager::RenderLightPass()
{
	m_d3d.SetDefaultTarget();
	m_deferredRenderingManager.RenderLightPass();
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
