#ifndef OBJECT_HPP
#define OBJECT_HPP
#include <DirectXMath.h>
#include "Vector3.hpp"
#include <string>

class Object
{
public:
	Object();
	virtual ~Object();

	virtual bool Initialize() = 0;

	void Update();

	void Move(const Vector3f& offset);
	void Move(float x, float y, float z);
	void Rotate(const Vector3f& offset);
	void Rotate(float x, float y, float z);

	void SetPosition(const Vector3f& position);
	void SetPosition(float x, float y, float z);
	void SetRotation(const Vector3f& rotation);
	void SetRotation(float x, float y, float z);
	void SetScale(const Vector3f& scale);
	void SetScale(float x, float y, float z);

	void SetModelName(const std::string& modelName);

	const Vector3f& GetPosition() const;
	const Vector3f& GetRotation() const;
	const Vector3f& GetScale() const;

	DirectX::XMMATRIX GetWorldMatrix() const;

	const std::string& GetModelName() const;

protected:
	void InitializeBaseClass();

	std::string m_modelName;

private:
	void UpdateWorldMatrix();

	Vector3f m_position;
	Vector3f m_rotation;
	Vector3f m_scale;

	DirectX::XMFLOAT4X4 m_worldMatrix;

	bool m_update_flag_world;
};

#endif