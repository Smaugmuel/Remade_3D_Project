#include "Matrix.hpp"
#include <algorithm>		// Fow swap()
#include <cstring>			// For memcpy()
#include <DirectXMath.h>	// For specific matrix functions

/*
DirectX::XMMATRIX convert functions
*/
Math::Matrix FromXMMATRIX(const DirectX::XMMATRIX & xm)
{
	Math::Matrix m;
	std::memcpy(m.rows, xm.r, 64U);
	return m;
}
DirectX::XMMATRIX ToXMMATRIX(const Math::Matrix & m)
{
	DirectX::XMMATRIX xm;
	std::memcpy(xm.r, m.rows, 64U);
	return xm;
}

/*
Member functions
*/
Math::Matrix::Matrix()
{
	rows[0][0] = 1.0f;
	rows[0][1] = 0.0f;
	rows[0][2] = 0.0f;
	rows[0][3] = 0.0f;
	rows[1][0] = 0.0f;
	rows[1][1] = 1.0f;
	rows[1][2] = 0.0f;
	rows[1][3] = 0.0f;
	rows[2][0] = 0.0f;
	rows[2][1] = 0.0f;
	rows[2][2] = 1.0f;
	rows[2][3] = 0.0f;
	rows[3][0] = 0.0f;
	rows[3][1] = 0.0f;
	rows[3][2] = 0.0f;
	rows[3][3] = 1.0f;
}
Math::Matrix::Matrix(const Math::Matrix & other)
{
	*this = other;
}
Math::Matrix::Matrix(float p00, float p01, float p02, float p03, float p10, float p11, float p12, float p13, float p20, float p21, float p22, float p23, float p30, float p31, float p32, float p33)
{
	rows[0][0] = p00;
	rows[0][1] = p01;
	rows[0][2] = p02;
	rows[0][3] = p03;
	rows[1][0] = p10;
	rows[1][1] = p11;
	rows[1][2] = p12;
	rows[1][3] = p13;
	rows[2][0] = p20;
	rows[2][1] = p21;
	rows[2][2] = p22;
	rows[2][3] = p23;
	rows[3][0] = p30;
	rows[3][1] = p31;
	rows[3][2] = p32;
	rows[3][3] = p33;
}
Math::Matrix::~Matrix()
{
}

Math::Matrix& Math::Matrix::operator=(const Math::Matrix & right)
{
	memcpy(rows, right.rows, 64U);
	return *this;
}
Math::Matrix& Math::Matrix::operator+=(const Math::Matrix & right)
{
	rows[0][0] += right.rows[0][0];
	rows[0][1] += right.rows[0][1];
	rows[0][2] += right.rows[0][2];
	rows[0][3] += right.rows[0][3];
	rows[1][0] += right.rows[1][0];
	rows[1][1] += right.rows[1][1];
	rows[1][2] += right.rows[1][2];
	rows[1][3] += right.rows[1][3];
	rows[2][0] += right.rows[2][0];
	rows[2][1] += right.rows[2][1];
	rows[2][2] += right.rows[2][2];
	rows[2][3] += right.rows[2][3];
	rows[3][0] += right.rows[3][0];
	rows[3][1] += right.rows[3][1];
	rows[3][2] += right.rows[3][2];
	rows[3][3] += right.rows[3][3];
	return *this;
}
Math::Matrix& Math::Matrix::operator-=(const Math::Matrix & right)
{
	rows[0][0] -= right.rows[0][0];
	rows[0][1] -= right.rows[0][1];
	rows[0][2] -= right.rows[0][2];
	rows[0][3] -= right.rows[0][3];
	rows[1][0] -= right.rows[1][0];
	rows[1][1] -= right.rows[1][1];
	rows[1][2] -= right.rows[1][2];
	rows[1][3] -= right.rows[1][3];
	rows[2][0] -= right.rows[2][0];
	rows[2][1] -= right.rows[2][1];
	rows[2][2] -= right.rows[2][2];
	rows[2][3] -= right.rows[2][3];
	rows[3][0] -= right.rows[3][0];
	rows[3][1] -= right.rows[3][1];
	rows[3][2] -= right.rows[3][2];
	rows[3][3] -= right.rows[3][3];
	return *this;
}
Math::Matrix& Math::Matrix::operator*=(const Math::Matrix & right)
{
	DirectX::XMMATRIX xm;
	DirectX::XMMATRIX xmr;

	std::memcpy(xm.r, rows, 64U);
	std::memcpy(xmr.r, right.rows, 64U);

	DirectX::XMMATRIX xmResult = DirectX::XMMatrixMultiply(xm, xmr);
	std::memcpy(rows, xmResult.r, 64U);

	return *this;
}
Math::Matrix& Math::Matrix::operator*=(float f)
{
	rows[0][0] *= f;
	rows[0][1] *= f;
	rows[0][2] *= f;
	rows[0][3] *= f;
	rows[1][0] *= f;
	rows[1][1] *= f;
	rows[1][2] *= f;
	rows[1][3] *= f;
	rows[2][0] *= f;
	rows[2][1] *= f;
	rows[2][2] *= f;
	rows[2][3] *= f;
	rows[3][0] *= f;
	rows[3][1] *= f;
	rows[3][2] *= f;
	rows[3][3] *= f;
	return *this;
}
Math::Matrix& Math::Matrix::operator/=(float f)
{
	float temp = 1.0f / f;
	rows[0][0] *= temp;
	rows[0][1] *= temp;
	rows[0][2] *= temp;
	rows[0][3] *= temp;
	rows[1][0] *= temp;
	rows[1][1] *= temp;
	rows[1][2] *= temp;
	rows[1][3] *= temp;
	rows[2][0] *= temp;
	rows[2][1] *= temp;
	rows[2][2] *= temp;
	rows[2][3] *= temp;
	rows[3][0] *= temp;
	rows[3][1] *= temp;
	rows[3][2] *= temp;
	rows[3][3] *= temp;
	return *this;
}

Math::Matrix Math::Matrix::operator+(const Math::Matrix & right) const
{
	return Math::Matrix(
		rows[0][0] + right.rows[0][0], rows[0][1] + right.rows[0][1], rows[0][2] + right.rows[0][2], rows[0][3] + right.rows[0][3],
		rows[1][0] + right.rows[1][0], rows[1][1] + right.rows[1][1], rows[1][2] + right.rows[1][2], rows[1][3] + right.rows[1][3],
		rows[2][0] + right.rows[2][0], rows[2][1] + right.rows[2][1], rows[2][2] + right.rows[2][2], rows[2][3] + right.rows[2][3],
		rows[3][0] + right.rows[3][0], rows[3][1] + right.rows[3][1], rows[3][2] + right.rows[3][2], rows[3][3] + right.rows[3][3]
	);
}
Math::Matrix Math::Matrix::operator-(const Math::Matrix & right) const
{
	return Math::Matrix(
		rows[0][0] - right.rows[0][0], rows[0][1] - right.rows[0][1], rows[0][2] - right.rows[0][2], rows[0][3] - right.rows[0][3],
		rows[1][0] - right.rows[1][0], rows[1][1] - right.rows[1][1], rows[1][2] - right.rows[1][2], rows[1][3] - right.rows[1][3],
		rows[2][0] - right.rows[2][0], rows[2][1] - right.rows[2][1], rows[2][2] - right.rows[2][2], rows[2][3] - right.rows[2][3],
		rows[3][0] - right.rows[3][0], rows[3][1] - right.rows[3][1], rows[3][2] - right.rows[3][2], rows[3][3] - right.rows[3][3]
	);
}
Math::Matrix Math::Matrix::operator*(const Math::Matrix & right) const
{
	DirectX::XMMATRIX xm;
	DirectX::XMMATRIX xmr;

	std::memcpy(xm.r, rows, 64U);
	std::memcpy(xmr.r, right.rows, 64U);
	
	DirectX::XMMATRIX xmResult = DirectX::XMMatrixMultiply(xm, xmr);
	Math::Matrix m;
	std::memcpy(m.rows, xmResult.r, 64U);

	return m;
}
Math::Matrix Math::Matrix::operator*(float f) const
{
	return Math::Matrix(
		rows[0][0] * f, rows[0][1] * f, rows[0][2] * f, rows[0][3] * f,
		rows[1][0] * f, rows[1][1] * f, rows[1][2] * f, rows[1][3] * f,
		rows[2][0] * f, rows[2][1] * f, rows[2][2] * f, rows[2][3] * f,
		rows[3][0] * f, rows[3][1] * f, rows[3][2] * f, rows[3][3] * f
	);
}
Math::Matrix Math::Matrix::operator/(float f) const
{
	float temp = 1.0f / f;

	return Math::Matrix(
		rows[0][0] * temp, rows[0][1] * temp, rows[0][2] * temp, rows[0][3] * temp,
		rows[1][0] * temp, rows[1][1] * temp, rows[1][2] * temp, rows[1][3] * temp,
		rows[2][0] * temp, rows[2][1] * temp, rows[2][2] * temp, rows[2][3] * temp,
		rows[3][0] * temp, rows[3][1] * temp, rows[3][2] * temp, rows[3][3] * temp
	);
}

bool Math::Matrix::operator==(const Math::Matrix & right) const
{
	if (rows[0][0] != right.rows[0][0]) return false;
	if (rows[0][1] != right.rows[0][1]) return false;
	if (rows[0][2] != right.rows[0][2]) return false;
	if (rows[0][3] != right.rows[0][3]) return false;
	if (rows[1][0] != right.rows[1][0]) return false;
	if (rows[1][1] != right.rows[1][1]) return false;
	if (rows[1][2] != right.rows[1][2]) return false;
	if (rows[1][3] != right.rows[1][3]) return false;
	if (rows[2][0] != right.rows[2][0]) return false;
	if (rows[2][1] != right.rows[2][1]) return false;
	if (rows[2][2] != right.rows[2][2]) return false;
	if (rows[2][3] != right.rows[2][3]) return false;
	if (rows[3][0] != right.rows[3][0]) return false;
	if (rows[3][1] != right.rows[3][1]) return false;
	if (rows[3][2] != right.rows[3][2]) return false;
	if (rows[3][3] != right.rows[3][3]) return false;
	
	return true;
}

void Math::Matrix::SetTranslation(const Vector3f & translation)
{
	DirectX::XMMATRIX xm = DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z);
	std::memcpy(rows, xm.r, 64U);
}
void Math::Matrix::SetScale(const Vector3f & scale)
{
	DirectX::XMMATRIX xm = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	std::memcpy(rows, xm.r, 64U);
}
void Math::Matrix::SetView(const Vector3f & position, const Vector3f & target, const Vector3f & up)
{
	DirectX::XMMATRIX xm = DirectX::XMMatrixLookAtLH({ position.x, position.y, position.z }, { target.x, target.y, target.z }, { up.x, up.y, up.z });
	std::memcpy(rows, xm.r, 64U);
}
void Math::Matrix::SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane)
{
	DirectX::XMMATRIX xm = DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane);
	std::memcpy(rows, xm.r, 64U);
}
void Math::Matrix::SetRotationAroundAxis(const Vector3f & axis, float angle)
{
	DirectX::XMMATRIX xm = DirectX::XMMatrixRotationAxis({ axis.x, axis.y, axis.z }, angle);
	std::memcpy(rows, xm.r, 64U);
}
void Math::Matrix::SetTranspose()
{
	std::swap(rows[0][1], rows[1][0]);
	std::swap(rows[0][2], rows[2][0]);
	std::swap(rows[0][3], rows[3][0]);
	std::swap(rows[1][2], rows[2][1]);
	std::swap(rows[1][3], rows[3][1]);
	std::swap(rows[2][3], rows[3][2]);
}
void Math::Matrix::SetIdentity()
{
	DirectX::XMMATRIX xm = DirectX::XMMatrixIdentity();
	std::memcpy(rows, xm.r, 64U);
}

Math::Matrix Math::Matrix::GetTranspose() const
{
	// Doing it this way resulted in an FPS of 64 with about 8100 objects,
	// as opposed to 49 when creating a new matrix and swapping all but the main diagonal
	return Math::Matrix(
		rows[0][0], rows[1][0], rows[2][0], rows[3][0],
		rows[0][1], rows[1][1], rows[2][1], rows[3][1],
		rows[0][2], rows[1][2], rows[2][2], rows[3][2],
		rows[0][3], rows[1][3], rows[2][3], rows[3][3]
	);
}

/*
Static member functions
*/
Math::Matrix Math::Matrix::Translation(const Vector3f & translation)
{
	Math::Matrix m;
	m.rows[3][0] = translation.x;
	m.rows[3][1] = translation.y;
	m.rows[3][2] = translation.z;

	return m;
}
Math::Matrix Math::Matrix::Rotation(const Vector3f & axis, float angle)
{
	Math::Matrix m;
	DirectX::XMMATRIX xm = DirectX::XMMatrixRotationAxis({ axis.x, axis.y, axis.z }, angle);
	std::memcpy(m.rows, xm.r, 64U);
	return m;
}
Math::Matrix Math::Matrix::Scale(const Vector3f & scale)
{
	Math::Matrix m;
	m.rows[0][0] = scale.x;
	m.rows[1][1] = scale.y;
	m.rows[2][2] = scale.z;
	return m;
}
Math::Matrix Math::Matrix::World(const Vector3f & position, const Matrix & rotation, const Vector3f & scale)
{
	Math::Matrix m = rotation;

	m.rows[0][0] *= scale.x;
	m.rows[1][1] *= scale.y;
	m.rows[2][2] *= scale.z;

	m.rows[3][0] = position.x;
	m.rows[3][1] = position.y;
	m.rows[3][2] = position.z;

	//return std::move(m);
	return m;
}
Math::Matrix Math::Matrix::View(const Vector3f & position, const Vector3f & target, const Vector3f & up)
{
	return FromXMMATRIX(DirectX::XMMatrixLookAtLH({ position.x, position.y, position.z }, { target.x, target.y, target.z }, { up.x, up.y, up.z }));
}
Math::Matrix Math::Matrix::Projection(float fov, float aspectRatio, float nearPlane, float farPlane)
{
	return FromXMMATRIX(DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane));
}