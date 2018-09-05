#ifndef MATRIX_HPP
#define MATRIX_HPP
#include "Vector3.hpp"
#include <DirectXMath.h>

/*
===============================================================================================================================
These are equivalent
-------------------------------------------------------------------------------------------------------------------------------
Matrix m = m0 * m1;
DirectX::XMMATRIX xm = xm0 * xm1;
DirectX::XMMATRIX xm2 = DirectX::XMMatrixMultiply(xm0, xm1);
-------------------------------------------------------------------------------------------------------------------------------
DirectX::XMMATRIX w1 = (DirectX::XMMatrixTranspose(t) * DirectX::XMMatrixTranspose(r)) * DirectX::XMMatrixTranspose(s);
DirectX::XMMATRIX w2 = DirectX::XMMatrixTranspose(s * (r * t));
===============================================================================================================================
These are not equivalent
-------------------------------------------------------------------------------------------------------------------------------
DirectX::XMMATRIX xm0 = DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtLH({ 1, 2, 3 }, { 0, 0, 0 }, { 0, 1, 0 }));
DirectX::XMMATRIX xm1 = DirectX::XMMatrixLookAtRH({ 1, 2, 3 }, { 0, 0, 0 }, { 0, 1, 0 });
-------------------------------------------------------------------------------------------------------------------------------


How are the matrices sent to the shader

world = DirectX::XMMatrixTranspose(s * (r * t));
view = DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtLH(pos, targ, up))
proj = DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(fov, ar, np, fp))

*/


struct Matrix
{
	Matrix();
	Matrix(const Matrix& other);
	Matrix(float p00, float p01, float p02, float p03, float p10, float p11, float p12, float p13, float p20, float p21, float p22, float p23, float p30, float p31, float p32, float p33);
	~Matrix();

	/*
	Assignment operators
	*/
	Matrix& operator=(const Matrix& right);
	Matrix& operator+=(const Matrix& right);
	Matrix& operator-=(const Matrix& right);
	Matrix& operator*=(const Matrix& right);
	Matrix& operator*=(float f);
	Matrix& operator/=(float f);

	/*
	Unaltering operators
	*/
	Matrix operator+(const Matrix& right) const;
	Matrix operator-(const Matrix& right) const;
	Matrix operator*(const Matrix& right) const;
	Matrix operator*(float f) const;
	Matrix operator/(float f) const;
	
	/*
	Comparison operators
	*/
	bool operator==(const Matrix& right) const;

	/*
	Assignment functions
	*/
	Matrix& SetTranslation(const Vector3f& translation);
	Matrix& SetScale(const Vector3f& scale);
	Matrix& SetView(const Vector3f& position, const Vector3f& target, const Vector3f& up);
	Matrix& SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane);
	Matrix& SetRotationAroundAxis(const Vector3f& axis, float angle);
	Matrix& SetTranspose();

	/*
	Unaltering functions
	*/
	Matrix GetTranspose() const;

	/*
	Static creation
	*/
	static Matrix Translation(const Vector3f& translation);
	static Matrix Rotation(const Vector3f& axis, float angle);
	static Matrix Scale(const Vector3f& scale);
	static Matrix View(const Vector3f& position, const Vector3f& target, const Vector3f& up);
	static Matrix Projection(float fov, float aspectRatio, float nearPlane, float farPlane);
	static Matrix Transpose(const Matrix& matrix);

	/*
	Data
	*/
	float rows[4][4];

private:
	/*
	DirectX conversions
	*/
	Matrix(const DirectX::XMMATRIX& m);
	Matrix& SetFromXMMATRIX(const DirectX::XMMATRIX& m);
	Matrix FromXMMATRIX(const DirectX::XMMATRIX& m) const;
	DirectX::XMMATRIX ToXMMATRIX() const;
};

#endif