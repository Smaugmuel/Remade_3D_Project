#ifndef OBJECT_HPP
#define OBJECT_HPP
#include "../../Math/Vector3.hpp"
#include "../../Math/OBB.hpp"
#include <DirectXMath.h>
#include <string>

class Object
{
protected:
	Object();
	virtual ~Object();

public:
	virtual bool Initialize();

	void Update();

	void Move(const Vector3f& offset);
	void Move(float x, float y, float z);
	void Rotate(const Vector3f& offset);
	void Rotate(float x, float y, float z);
	void RotateLocal(const Vector3f& vec, float angle);
	void RotateLocal(float x, float y, float z, float angle);
	void RotateGlobal(const Vector3f& vec, float angle);
	void RotateGlobal(float x, float y, float z, float angle);

	virtual void SetPosition(const Vector3f& position);
	virtual void SetPosition(float x, float y, float z);
	void SetRotation(const Vector3f& rotation);
	void SetRotation(float x, float y, float z);
	void SetScale(const Vector3f& scale);
	void SetScale(float x, float y, float z);

	//void SetModelName(const std::string& modelName);

	const Vector3f& GetPosition() const;
	const Vector3f& GetRotation() const;
	const Vector3f& GetScale() const;

	DirectX::XMMATRIX GetRotationMatrix() const;
	DirectX::XMMATRIX GetWorldMatrix() const;

	//const std::string& GetModelName() const;

	const OBB& GetOBB() const;

protected:
	//std::string m_modelName;

	OBB m_obb;

private:
	void UpdateTranslationMatrix();
	void UpdateRotationMatrixFromEuler();
	void UpdateRotationEulerFromMatrix();
	void UpdateScalingMatrix();
	void UpdateWorldMatrix();

	virtual void UpdateOBB() = 0;
	
	Vector3f m_position;
	Vector3f m_rotation;
	Vector3f m_scale;

	DirectX::XMFLOAT4X4 m_translationMatrix;
	DirectX::XMFLOAT4X4 m_rotationMatrix;
	DirectX::XMFLOAT4X4 m_scalingMatrix;
	DirectX::XMFLOAT4X4 m_worldMatrix;
	
	bool m_update_translation_flag;
	bool m_update_rotation_from_euler_flag;
	bool m_update_rotation_from_matrix_flag;
	bool m_update_scaling_flag;
	bool m_update_world_flag;
};

#endif