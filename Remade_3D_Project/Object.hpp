#ifndef OBJECT_HPP
#define OBJECT_HPP
#include <DirectXMath.h>
#include "Vector3.hpp"
#include <memory>

struct ID3D11Device;
struct ID3D11DeviceContext;

class Model;
class Camera;

class Object
{
public:
	Object();
	~Object();

	bool Initialize(ID3D11Device* device, const char* modelFileName);

	void Update();
	void Render(ID3D11DeviceContext* deviceContext);

	void Move(const Vector3f& offset);
	void Move(const float& x, const float& y, const float& z);
	void Rotate(const Vector3f& offset);
	void Rotate(const float& x, const float& y, const float& z);

	void SetPosition(const Vector3f& position);
	void SetPosition(const float& x, const float& y, const float& z);
	void SetRotation(const Vector3f& rotation);
	void SetRotation(const float& x, const float& y, const float& z);
	void SetScale(const Vector3f& scale);
	void SetScale(const float& x, const float& y, const float& z);

	const Vector3f& GetPosition() const;
	const Vector3f& GetRotation() const;
	const Vector3f& GetScale() const;

	const DirectX::XMMATRIX GetWorldMatrix() const;

private:
	void UpdateWorldMatrix();

	Vector3f m_position;
	Vector3f m_rotation;
	Vector3f m_scale;

	DirectX::XMFLOAT4X4 m_worldMatrix;

	std::unique_ptr<Model> m_model;

	bool m_update_flag_world;
};

#endif