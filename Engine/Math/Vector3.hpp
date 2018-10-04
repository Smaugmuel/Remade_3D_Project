#ifndef VECTOR3_HPP
#define VECTOR3_HPP
#include <math.h>

struct Vector3f final
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	Vector3f() : x(0.0f), y(0.0f), z(0.0f)
	{
	}
	Vector3f(float pX, float pY, float pZ) : x(pX), y(pY), z(pZ)
	{
	}
	Vector3f(const Vector3f& right) : x(right.x), y(right.y), z(right.z)
	{
	}
	~Vector3f()
	{
	}

	Vector3f& operator=(const Vector3f& right)
	{
		x = right.x;
		y = right.y;
		z = right.z;
		return *this;
	}
	Vector3f& operator+=(const Vector3f& right)
	{
		x += right.x;
		y += right.y;
		z += right.z;
		return *this;
	}
	Vector3f& operator-=(const Vector3f& right)
	{
		x -= right.x;
		y -= right.y;
		z -= right.z;
		return *this;
	}
	Vector3f& operator*=(float coefficient)
	{
		x *= coefficient;
		y *= coefficient;
		z *= coefficient;
		return *this;
	}
	Vector3f& operator/=(float coefficient)
	{
		float temp = 1.0f / coefficient;
		x *= temp;
		y *= temp;
		z *= temp;
		return *this;
	}

	Vector3f operator+(const Vector3f& right) const
	{
		return Vector3f(x + right.x, y + right.y, z + right.z);
	}
	Vector3f operator-(const Vector3f& right) const
	{
		return Vector3f(x - right.x, y - right.y, z - right.z);
	}
	Vector3f operator*(float coefficient) const
	{
		return Vector3f(x * coefficient, y * coefficient, z * coefficient);
	}
	Vector3f operator/(float coefficient) const
	{
		float temp = 1.0f / coefficient;
		return Vector3f(x * temp, y * temp, z * temp);
	}

	bool operator==(const Vector3f& right) const
	{
		return x == right.x && y == right.y && z == right.z;
	}

	Vector3f normalized() const
	{
		float temp = 1.0f / sqrtf(x * x + y * y + z * z);
		return Vector3f(x * temp, y * temp, z * temp);
	}
	void normalize()
	{
		float temp = 1.0f / sqrtf(x * x + y * y + z * z);
		x *= temp;
		y *= temp;
		z *= temp;
	}
	void setLength(float length)
	{
		float temp = length / sqrtf(x * x + y * y + z * z);
		x *= temp;
		y *= temp;
		z *= temp;
	}

	float lengthSquared() const
	{
		return (x*x + y*y + z*z);
	}
	float length() const
	{
		return sqrtf(x*x + y * y + z * z);
	}

	float dot(const Vector3f& right) const
	{
		return x * right.x + y * right.y + z * right.z;
	}
	Vector3f crossRH(const Vector3f& vec) const
	{
		return Vector3f(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x);
	}
	Vector3f crossLH(const Vector3f& vec) const
	{
		return Vector3f(z*vec.y - y*vec.z, x*vec.z - z*vec.x, y*vec.x - x*vec.y);
	}
	Vector3f reflectThisAround(const Vector3f& vec) const
	{
		return (vec * (2 * dot(vec) / vec.lengthSquared()) - *this);
	}
	Vector3f reflectThisOn(const Vector3f& mirror) const
	{
		return (*this - mirror * (2 * dot(mirror) / mirror.lengthSquared()));
	}
	float ProjectPointOnThis(const Vector3f& point) const
	{
		return dot(point);
	}
};

#endif