#ifndef FRAMEWORK_MANAGER_HPP
#define FRAMEWORK_MANAGER_HPP
#include "Window.hpp"
#include "Input.hpp"
#include "Direct3D.hpp"
#include "ConstantBufferManager.hpp"
#include "VertexBufferManager.hpp"
#include "ShaderManagerV2.hpp"
#include "TextureManager.hpp"
#include "GUIManager.hpp"
#include "SamplerManager.hpp"
#include "DeferredRenderingManager.hpp"
#include "ShadowMapRenderingManager.hpp"
#include "OnScreenTarget.hpp"

class FrameWorkManager final
{
public:
	FrameWorkManager();
	~FrameWorkManager();

#ifdef USES_MATRIX_BUFFER
	bool Initialize(Vector2i windowSize, int maxNrOfLights, int nrOfWorldMatricesInBuffer);
#else
	bool Initialize(Vector2i windowSize, int maxNrOfLights);
#endif

	void SetGeometryPassRenderTargets();
	void SetLightPassRenderTarget();
	void SetShadowPassRenderTarget();
	void SetFinalPassRenderTarget();

	void ClearGeometryPassRenderTargets(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
	void ClearLightPassRenderTargets(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
	void ClearShadowPassRenderTargets();
	void ClearFinalPassRenderTarget(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
	
	void RenderGeometryPassWithCurrentSettings(int nrOfVertices);
	void RenderLightPass();
	void RenderFinalPass();
	
	void Present();

	Window* GetWindow();
	Input* GetInput();
	Direct3D* GetDirect3D();
	ConstantBufferManager* GetConstantBufferManager();
	VertexBufferManager* GetVertexBufferManager();
	ShaderManagerV2* GetShaderManager();
	TextureManager* GetTextureManager();
	GUIManager* GetGUIManager();
	SamplerManager* GetSamplerManager();

private:
	Window m_window;
	Input m_input;
	Direct3D m_d3d;
	ConstantBufferManager m_constantBufferManager;
	VertexBufferManager m_vertexBufferManager;
	ShaderManagerV2 m_shaderManager;
	TextureManager m_textureManager;
	GUIManager m_guiManager;
	SamplerManager m_samplerManager;
	DeferredRenderingManager m_deferredRenderingManager;
	ShadowMapRenderingManager m_shadowMapRenderingManager;
	OnScreenTarget m_onScreenTarget;
};

#endif