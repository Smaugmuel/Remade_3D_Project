#ifndef SHADOW_MAP_RENDERING_MANAGER_HPP
#define SHADOW_MAP_RENDERING_MANAGER_HPP
#include "../Math/Vector2.hpp"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11ShaderResourceView;
struct ID3D11Texture2D;
struct ID3D11DepthStencilView;

class ShadowMapRenderingManager final
{
public:
	ShadowMapRenderingManager();
	~ShadowMapRenderingManager();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const Vector2i& windowDimensions);

	void ClearDepthStencilView();
	void SetRenderTarget();

	ID3D11ShaderResourceView* GetShaderResourceView();

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;

	ID3D11ShaderResourceView * m_shaderResourceView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilView* m_depthStencilView;
};

#endif