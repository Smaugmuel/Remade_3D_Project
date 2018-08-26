#include "Matrix.hpp"

Matrix::Matrix()
{
	for (unsigned int i = 0; i < 4; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			rows[i][j] = 0.0f;
		}
	}
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

Matrix & Matrix::operator=(const Matrix & right)
{
	for (unsigned int i = 0; i < 4; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			rows[i][j] = right.rows[i][j];
		}
	}

	return *this;
}
Matrix& Matrix::operator+=(const Matrix & right)
{
	for (unsigned int i = 0; i < 4; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			rows[i][j] += right.rows[i][j];
		}
	}

	return *this;
}
Matrix& Matrix::operator-=(const Matrix & right)
{
	for (unsigned int i = 0; i < 4; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			rows[i][j] -= right.rows[i][j];
		}
	}

	return *this;
}
Matrix& Matrix::operator*=(const Matrix & right)
{
	SetFromXMMATRIX(ToXMMATRIX() * right.ToXMMATRIX());

	return *this;
}
Matrix& Matrix::operator*=(float f)
{
	for (unsigned int i = 0; i < 4; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			rows[i][j] *= f;
		}
	}

	return *this;
}
Matrix& Matrix::operator/=(float f)
{
	*this *= (1.0f / f);

	return *this;
}

Matrix Matrix::operator+(const Matrix & right) const
{
	Matrix m = *this;
	m += right;
	return m;
}
Matrix Matrix::operator-(const Matrix & right) const
{
	Matrix m = *this;
	m -= right;
	return m;
}
Matrix Matrix::operator*(const Matrix & right) const
{
	Matrix m = *this;
	m *= right;
	return m;
}
Matrix Matrix::operator*(float f) const
{
	Matrix m = *this;
	m *= f;
	return m;
}
Matrix Matrix::operator/(float f) const
{
	Matrix m = *this;
	m *= (1.0f / f);
	return m;
}

bool Matrix::operator==(const Matrix & right) const
{
	for (unsigned int i = 0; i < 4; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			if (rows[i][j] != right.rows[i][j])
			{
				return false;
			}
		}
	}
	return true;
}

Matrix & Matrix::SetTranslation(const Vector3f & translation)
{
	SetFromXMMATRIX(DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z));
	return *this;
}
Matrix & Matrix::SetScale(const Vector3f & scale)
{
	SetFromXMMATRIX(DirectX::XMMatrixScaling(scale.x, scale.y, scale.z));
	return *this;
}
Matrix & Matrix::SetView(const Vector3f & position, const Vector3f & target, const Vector3f & up)
{
	SetFromXMMATRIX(DirectX::XMMatrixLookAtLH(position.XMV(), target.XMV(), up.XMV()));
	return *this;
}
Matrix & Matrix::SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane)
{
	SetFromXMMATRIX(DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane));
	return *this;
}
Matrix & Matrix::SetRotationAroundAxis(const Vector3f & axis, float angle)
{
	SetFromXMMATRIX(DirectX::XMMatrixRotationAxis(axis.XMV(), angle));
	return *this;
}
Matrix & Matrix::SetTranspose()
{
	SetFromXMMATRIX(DirectX::XMMatrixTranspose(ToXMMATRIX()));
	return *this;
}

Matrix Matrix::GetTranspose() const
{
	return Matrix(*this).SetTranspose();
}

Matrix & Matrix::SetFromXMMATRIX(const DirectX::XMMATRIX & m)
{
	for (unsigned int i = 0; i < 4; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			rows[i][j] = m.r[i].m128_f32[j];
		}
	}
	return *this;
}
Matrix Matrix::FromXMMATRIX(const DirectX::XMMATRIX & m) const
{
	Matrix _m;
	_m.SetFromXMMATRIX(m);
	return _m;
}
DirectX::XMMATRIX Matrix::ToXMMATRIX() const
{
	return DirectX::XMMATRIX(rows[0][0], rows[0][1], rows[0][2], rows[0][3], rows[1][0], rows[1][1], rows[1][2], rows[1][3], rows[2][0], rows[2][1], rows[2][2], rows[2][3], rows[3][0], rows[3][1], rows[3][2], rows[3][3]);
}