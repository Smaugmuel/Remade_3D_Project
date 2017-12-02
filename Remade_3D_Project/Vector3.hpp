#ifndef VECTOR3_HPP
#define VECTOR3_HPP
#include <DirectXMath.h>

template<typename Type>
struct Vector3 final
{
	Type x;
	Type y;
	Type z;

	Vector3() : x(0), y(0), z(0)
	{
	}
	Vector3(const Type& pX, const Type& pY, const Type& pZ) : x(pX), y(pY), z(pZ)
	{
	}
	Vector3(const Vector3<Type>& right) : x(right.x), y(right.y), z(right.z)
	{
	}
	Vector3(const DirectX::XMFLOAT3& right) : x(right.x), y(right.y), z(right.z)
	{
	}
	Vector3(const DirectX::XMVECTOR& right)
	{
		DirectX::XMFLOAT3 v;
		DirectX::XMStoreFloat3(&v, right);
	
		*this = v;
	}
	~Vector3()
	{
	}

	const Vector3<Type> operator=(const Vector3<Type>& right)
	{
		x = right.x;
		y = right.y;
		z = right.z;

		return *this;
	}
	const Vector3<Type> operator=(const DirectX::XMFLOAT3& right)
	{
		return *this = Vector3<Type>(right);
	}
	const Vector3<Type> operator+=(const Vector3<Type>& right)
	{
		x += right.x;
		y += right.y;
		z += right.z;

		return *this;
	}
	const Vector3<Type> operator+=(const DirectX::XMFLOAT3& right)
	{
		return *this += Vector3<Type>(right);
	}
	const Vector3<Type> operator-=(const Vector3<Type>& right)
	{
		x -= right.x;
		y -= right.y;
		z -= right.z;

		return *this;
	}
	const Vector3<Type> operator-=(const DirectX::XMFLOAT3& right)
	{
		return *this -= Vector3<Type>(right);
	}
	const Vector3<Type> operator*=(const Type& coefficient)
	{
		x *= coefficient;
		y *= coefficient;
		z *= coefficient;

		return *this;
	}

	const Vector3<Type> operator+(const Vector3<Type>& right) const
	{
		return Vector3<Type>(x + right.x, y + right.y, z + right.z);
	}
	const Vector3<Type> operator+(const DirectX::XMFLOAT3& right) const
	{
		return *this + Vector3<Type>(right);
	}
	const Vector3<Type> operator-(const Vector3<Type>& right) const
	{
		return Vector3<Type>(x - right.x, y - right.y, z - right.z);
	}
	const Vector3<Type> operator-(const DirectX::XMFLOAT3& right) const
	{
		return *this - Vector3<Type>(right);
	}
	const Vector3<Type> operator*(const Type& coefficient) const
	{
		return Vector3<Type>(x * coefficient, y * coefficient, z * coefficient);
	}

	bool operator==(const Vector3<Type>& right) const
	{
		return x == right.x && y == right.y && z == right.z;
	}
	bool operator==(const DirectX::XMFLOAT3& right) const
	{
		return *this == Vector3<Type>(right);
	}

	void normalize()
	{
		Type len = length();

		if (len == 0.0f)
			return;

		Type lenDiv = 1 / len;
		x *= lenDiv;
		y *= lenDiv;
		z *= lenDiv;
	}
	const Vector3<Type> normalized() const
	{
		Vector3<Type> v = *this;
		v.normalize();
		return v;
	}
	void setLength(const Type& _length)
	{
		normalize();
		this->operator*=(_length);
	}

	const Type lengthSquared() const
	{
		return (x*x + y*y + z*z);
	}
	const Type length() const
	{
		return sqrtf(lengthSquared());
	}

	const Type dot(const Vector3<Type>& right) const
	{
		return x*right.x + y*right.y + z*right.z;
	}
	const Type dot(const DirectX::XMFLOAT3& right) const
	{
		return dot(Vector3<Type>(right));
	}
	const Vector3<Type> crossRH(const Vector3<Type>& vec) const
	{
		return Vector3<Type>(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x);
	}
	const Vector3<Type> crossRH(const DirectX::XMFLOAT3& vec) const
	{
		return cross(Vector3<Type>(right));
	}
	const Vector3<Type> crossLH(const Vector3<Type>& vec) const
	{
		return Vector3<Type>(z*vec.y - y*vec.z, x*vec.z - z*vec.x, y*vec.x - x*vec.y);
	}
	const Vector3<Type> crossLH(const DirectX::XMFLOAT3& vec) const
	{
		return cross(Vector3<Type>(right));
	}
	const Vector3<Type> reflectThisAround(const Vector3<Type>& vec) const
	{
		return (vec * (2 * this->dot(vec) / vec.lengthSquared()) - *this);
	}
	const Vector3<Type> reflectThisAround(const DirectX::XMFLOAT3& vec) const
	{
		return reflectThisAround(Vector3<Type>(right));
	}
	const Vector3<Type> reflectThisOn(const Vector3<Type>& mirror) const
	{
		return (*this - mirror * (2 * this->dot(mirror) / mirror.lengthSquared()));
	}
	const Vector3<Type> reflectThisOn(const DirectX::XMFLOAT3& mirror) const
	{
		return reflectThisOn(Vector3<Type>(mirror));
	}

	const DirectX::XMFLOAT3 XMF() const
	{
		return DirectX::XMFLOAT3(x, y, z);
	}
	const DirectX::XMVECTOR XMV() const
	{
		DirectX::XMFLOAT3 vec = XMF();
		return DirectX::XMLoadFloat3(&vec);
	}
};

typedef Vector3<float> Vector3f;
typedef Vector3<int> Vector3i;

#endif