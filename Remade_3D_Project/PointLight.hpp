#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP
#include "Vector2.hpp"
#include "Vector3.hpp"
#include <DirectXMath.h>

struct ID3D11Device;
struct ID3D11DeviceContext;

struct ID3D11Texture2D;
struct ID3D11DepthStencilView;
struct ID3D11ShaderResourceView;
struct D3D11_VIEWPORT;

class Camera;

class PointLight
{
public:
	PointLight();
	~PointLight();

	bool Initialize(ID3D11Device* device, Vector2f dimensions);
	
	void Update();

	void Clear(ID3D11DeviceContext* deviceContext);
	void Set(ID3D11DeviceContext* deviceContext);

	void SetPosition(Vector3f position);
	void SetTarget(Vector3f target);

	Vector3f GetPosition() const;
	Vector3f GetTarget() const;

	DirectX::XMMATRIX GetViewMatrix() const;
	DirectX::XMMATRIX GetProjectionMatrix() const;

	ID3D11ShaderResourceView* GetDepthResourceView();

private:
	Camera* m_camera;

	ID3D11Texture2D* m_depthBuffer;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11ShaderResourceView* m_depthResourceView;
	D3D11_VIEWPORT* m_viewport;
};

#endif