#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <DirectXMath.h>
//#include "Frustum.hpp"
#include "../Math/Vector3.hpp"
#include "../Math/Vector2.hpp"

DirectX::XMVECTOR To_XMVECTOR(DirectX::XMFLOAT3 v);
DirectX::XMFLOAT3 To_XMFLOAT3(DirectX::XMVECTOR v);

class Camera
{
public:
	Camera();
	Camera(const Camera& camera);
	~Camera();

	void operator=(const Camera& camera);

	void Initialize();

	void Rotate(const Vector3f& axis, float angle);
	void RotateUp(float angle);
	void RotateRight(float angle);

	void Move(const Vector3f& direction);
	void MoveForward(float amount);
	void MoveRight(float amount);
	void MoveUp(float amount);

	void SetPosition(float x, float y, float z);
	void SetPosition(const Vector3f& position);
	void SetTarget(float x, float y, float z);
	void SetTarget(const Vector3f& target);

	void SetFOV(float FOV);
	void SetDimensions(const Vector2f& dimensions);
	void SetNearPlane(float nearPlane);
	void SetFarPlane(float farPlane);

	//void SetUnaltered();

	//void CreateFrustum();

	const Vector3f& GetPosition() const;
	const Vector3f& GetTarget() const;
	const Vector3f GetTargetDirection() const;
	const Vector3f GetForward() const;
	const Vector3f GetRight() const;
	
	const DirectX::XMMATRIX GetViewMatrix() const;
	const DirectX::XMMATRIX GetProjectionMatrix() const;
	const DirectX::XMMATRIX GetOrthogonalMatrix() const;


	float GetFOV() const;
	const Vector2f& GetDimensions() const;
	float GetNearPlane() const;
	float GetFarPlane() const;

	//bool GetWasAltered() const;
	//Frustum* GetViewFrustum();

	void Update();

private:
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();
	void UpdateOrthographicMatrix();


	Vector3f m_position;
	Vector3f m_target;
	Vector3f m_up;

	float m_FOV;
	Vector2f m_dimensions;
	float m_nearPlane;
	float m_farPlane;

	DirectX::XMFLOAT4X4 m_view;
	DirectX::XMFLOAT4X4 m_projection;
	DirectX::XMFLOAT4X4 m_orthographic;

	//Frustum* m_frustum;

	//bool m_wasAltered;
	bool m_update_flag_view;
	bool m_update_flag_proj;
};
#endif