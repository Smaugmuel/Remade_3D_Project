#include "Matrix.hpp"
#include <algorithm>	// Fow swap()
#include <cstring>		// For memcpy()

Matrix::Matrix()
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
Matrix::Matrix(const Matrix & other)
{
	*this = other;
}
Matrix::Matrix(float p00, float p01, float p02, float p03, float p10, float p11, float p12, float p13, float p20, float p21, float p22, float p23, float p30, float p31, float p32, float p33)
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
Matrix::~Matrix()
{
}

Matrix& Matrix::operator=(const Matrix & right)
{
	memcpy(rows, right.rows, 64U);
	return *this;
}
Matrix& Matrix::operator+=(const Matrix & right)
{
	return *this = *this + right;
}
Matrix& Matrix::operator-=(const Matrix & right)
{
	return *this = *this + right;
}
Matrix& Matrix::operator*=(const Matrix & right)
{
	return *this = *this * right;
}
Matrix& Matrix::operator*=(float f)
{
	return *this = *this * f;
}
Matrix& Matrix::operator/=(float f)
{
	return *this = *this / f;
}

Matrix Matrix::operator+(const Matrix & right) const
{
	Matrix m = *this;

	m.rows[0][0] += right.rows[0][0];
	m.rows[0][1] += right.rows[0][1];
	m.rows[0][2] += right.rows[0][2];
	m.rows[0][3] += right.rows[0][3];
	m.rows[1][0] += right.rows[1][0];
	m.rows[1][1] += right.rows[1][1];
	m.rows[1][2] += right.rows[1][2];
	m.rows[1][3] += right.rows[1][3];
	m.rows[2][0] += right.rows[2][0];
	m.rows[2][1] += right.rows[2][1];
	m.rows[2][2] += right.rows[2][2];
	m.rows[2][3] += right.rows[2][3];
	m.rows[3][0] += right.rows[3][0];
	m.rows[3][1] += right.rows[3][1];
	m.rows[3][2] += right.rows[3][2];
	m.rows[3][3] += right.rows[3][3];

	return m;
}
Matrix Matrix::operator-(const Matrix & right) const
{
	Matrix m = *this;

	m.rows[0][0] -= right.rows[0][0];
	m.rows[0][1] -= right.rows[0][1];
	m.rows[0][2] -= right.rows[0][2];
	m.rows[0][3] -= right.rows[0][3];
	m.rows[1][0] -= right.rows[1][0];
	m.rows[1][1] -= right.rows[1][1];
	m.rows[1][2] -= right.rows[1][2];
	m.rows[1][3] -= right.rows[1][3];
	m.rows[2][0] -= right.rows[2][0];
	m.rows[2][1] -= right.rows[2][1];
	m.rows[2][2] -= right.rows[2][2];
	m.rows[2][3] -= right.rows[2][3];
	m.rows[3][0] -= right.rows[3][0];
	m.rows[3][1] -= right.rows[3][1];
	m.rows[3][2] -= right.rows[3][2];
	m.rows[3][3] -= right.rows[3][3];

	return m;
}
Matrix Matrix::operator*(const Matrix & right) const
{
	DirectX::XMMATRIX xm;
	DirectX::XMMATRIX xmr;

	std::memcpy(xm.r, rows, 64U);
	std::memcpy(xmr.r, right.rows, 64U);
	
	DirectX::XMMATRIX xmResult = DirectX::XMMatrixMultiply(xm, xmr);
	//DirectX::XMMATRIX xmResult = xm * xmr;
	Matrix m;
	std::memcpy(m.rows, xmResult.r, 64U);

	return m;
}
Matrix Matrix::operator*(float f) const
{
	Matrix m = *this;
	
	m.rows[0][0] *= f;
	m.rows[0][1] *= f;
	m.rows[0][2] *= f;
	m.rows[0][3] *= f;
	m.rows[1][0] *= f;
	m.rows[1][1] *= f;
	m.rows[1][2] *= f;
	m.rows[1][3] *= f;
	m.rows[2][0] *= f;
	m.rows[2][1] *= f;
	m.rows[2][2] *= f;
	m.rows[2][3] *= f;
	m.rows[3][0] *= f;
	m.rows[3][1] *= f;
	m.rows[3][2] *= f;
	m.rows[3][3] *= f;

	return m;
}
Matrix Matrix::operator/(float f) const
{
	Matrix m = *this;
	float fDiv = 1.0f / f;

	m.rows[0][0] *= fDiv;
	m.rows[0][1] *= fDiv;
	m.rows[0][2] *= fDiv;
	m.rows[0][3] *= fDiv;
	m.rows[1][0] *= fDiv;
	m.rows[1][1] *= fDiv;
	m.rows[1][2] *= fDiv;
	m.rows[1][3] *= fDiv;
	m.rows[2][0] *= fDiv;
	m.rows[2][1] *= fDiv;
	m.rows[2][2] *= fDiv;
	m.rows[2][3] *= fDiv;
	m.rows[3][0] *= fDiv;
	m.rows[3][1] *= fDiv;
	m.rows[3][2] *= fDiv;
	m.rows[3][3] *= fDiv;

	return m;
}

bool Matrix::operator==(const Matrix & right) const
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

void Matrix::SetTranslation(const Vector3f & translation)
{
	SetFromXMMATRIX(DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z));
}
void Matrix::SetScale(const Vector3f & scale)
{
	SetFromXMMATRIX(DirectX::XMMatrixScaling(scale.x, scale.y, scale.z));
}
void Matrix::SetView(const Vector3f & position, const Vector3f & target, const Vector3f & up)
{
	SetFromXMMATRIX(DirectX::XMMatrixLookAtLH({ position.x, position.y, position.z }, { target.x, target.y, target.z }, { up.x, up.y, up.z }));
}
void Matrix::SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane)
{
	SetFromXMMATRIX(DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane));
}
void Matrix::SetRotationAroundAxis(const Vector3f & axis, float angle)
{
	SetFromXMMATRIX(DirectX::XMMatrixRotationAxis({ axis.x, axis.y, axis.z }, angle));
}
void Matrix::SetTranspose()
{
	std::swap(rows[0][1], rows[1][0]);
	std::swap(rows[0][2], rows[2][0]);
	std::swap(rows[0][3], rows[3][0]);
	std::swap(rows[1][2], rows[2][1]);
	std::swap(rows[1][3], rows[3][1]);
	std::swap(rows[2][3], rows[3][2]);
}
void Matrix::SetIdentity()
{
	SetFromXMMATRIX(DirectX::XMMatrixIdentity());
}

Matrix Matrix::GetTranspose() const
{
	return Matrix(
		rows[0][0], rows[1][0], rows[2][0], rows[3][0],
		rows[0][1], rows[1][1], rows[2][1], rows[3][1],
		rows[0][2], rows[1][2], rows[2][2], rows[3][2],
		rows[0][3], rows[1][3], rows[2][3], rows[3][3]
	);
}

Matrix Matrix::Translation(const Vector3f & translation)
{
	return FromXMMATRIX(DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z));
}
Matrix Matrix::Rotation(const Vector3f & axis, float angle)
{
	return FromXMMATRIX(DirectX::XMMatrixRotationAxis({ axis.x, axis.y, axis.z }, angle));
}
Matrix Matrix::Scale(const Vector3f & scale)
{
	return FromXMMATRIX(DirectX::XMMatrixScaling(scale.x, scale.y, scale.z));
}
Matrix Matrix::World(const Vector3f & position, const Matrix & rotation, const Vector3f & scale)
{
	Matrix m = rotation;
	
	m.rows[0][0] *= scale.x;
	m.rows[1][1] *= scale.y;
	m.rows[2][2] *= scale.z;

	m.rows[3][0] = position.x;
	m.rows[3][1] = position.y;
	m.rows[3][2] = position.z;

	//return std::move(m);
	return m;
}
Matrix Matrix::View(const Vector3f & position, const Vector3f & target, const Vector3f & up)
{
	return FromXMMATRIX(DirectX::XMMatrixLookAtLH({ position.x, position.y, position.z }, { target.x, target.y, target.z }, { up.x, up.y, up.z }));
}
Matrix Matrix::Projection(float fov, float aspectRatio, float nearPlane, float farPlane)
{
	return FromXMMATRIX(DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane));
}
Matrix Matrix::Transpose(const Matrix & matrix)
{
	return matrix.GetTranspose();
}

Matrix Matrix::FromXMMATRIX(const DirectX::XMMATRIX & xm)
{
	Matrix m;
	std::memcpy(m.rows, xm.r, 64U);
	return m;
}
void Matrix::SetFromXMMATRIX(const DirectX::XMMATRIX & xm)
{
	std::memcpy(rows, xm.r, 64U);
}
DirectX::XMMATRIX Matrix::ToXMMATRIX(const Matrix & m)
{
	DirectX::XMMATRIX xm;
	std::memcpy(xm.r, m.rows, 64U);
	return xm;
}