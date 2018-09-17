#ifndef VECTOR2_HPP
#define VECTOR2_HPP
#include <math.h>

struct Vector2i final
{
	int x = 0;
	int y = 0;

	Vector2i() : x(0), y(0)
	{

	}
	Vector2i(int pX, int pY) : x(pX), y(pY)
	{
	}
	Vector2i(const Vector2i& right) : x(right.x), y(right.y)
	{
	}
	~Vector2i()
	{
	}

	Vector2i& operator=(const Vector2i& right)
	{
		x = right.x;
		y = right.y;

		return *this;
	}
	Vector2i& operator+=(const Vector2i& right)
	{
		return *this = *this + right;
	}
	Vector2i& operator-=(const Vector2i& right)
	{
		return *this = *this - right;
	}
	Vector2i& operator*=(float coefficient)
	{
		return *this = *this * coefficient;
	}

	Vector2i operator+(const Vector2i& right) const
	{
		return Vector2i(x + right.x, y + right.y);
	}
	Vector2i operator-(const Vector2i& right) const
	{
		return Vector2i(x - right.x, y - right.y);
	}
	Vector2i operator*(float coefficient) const
	{
		return Vector2i(static_cast<int>(x * coefficient), static_cast<int>(y * coefficient));
	}
	Vector2i operator/(float coefficient) const
	{
		return coefficient == 0.0f ? *this : *this * (1.0f / coefficient);
	}
	bool operator==(const Vector2i& right) const
	{
		return right.x == x && right.y == y;
	}
	bool operator!=(const Vector2i& right) const
	{
		return !(*this == right);
	}

	float dot(const Vector2i& right) const
	{
		return static_cast<float>(x * right.x + y * right.y);
	}
	Vector2i cross() const
	{
		return Vector2i(-y, x);
	}

	float lengthSquared() const
	{
		return static_cast<float>(x*x + y * y);
	}
	float length() const
	{
		return sqrtf(lengthSquared());
	}
};

struct Vector2f final
{
	float x = 0.0f;
	float y = 0.0f;

	Vector2f() : x(0.0f), y(0.0f)
	{
	}
	Vector2f(float pX, float pY) : x(pX), y(pY)
	{
	}
	Vector2f(const Vector2f& right) : x(right.x), y(right.y)
	{
	}
	~Vector2f()
	{
	}

	Vector2f& operator=(const Vector2f& right)
	{
		x = right.x;
		y = right.y;

		return *this;
	}
	Vector2f& operator+=(const Vector2f& right)
	{
		return *this = *this + right;
	}
	Vector2f& operator-=(const Vector2f& right)
	{
		return *this = *this - right;
	}
	Vector2f& operator*=(float coefficient)
	{
		return *this = *this * coefficient;
	}
	Vector2f& operator/=(float coefficient)
	{
		return *this = *this / coefficient;
	}

	Vector2f operator+(const Vector2f& right) const
	{
		return Vector2f(x + right.x, y + right.y);
	}
	Vector2f operator-(const Vector2f& right) const
	{
		return Vector2f(x - right.x, y - right.y);
	}
	Vector2f operator*(float coefficient) const
	{
		return Vector2f(x * coefficient, y * coefficient);
	}
	Vector2f operator/(float coefficient) const
	{
		return coefficient == 0.0f ? *this : *this * (1.0f / coefficient);
	}
	bool operator==(const Vector2f& right) const
	{
		return right.x == x && right.y == y;
	}
	bool operator!=(const Vector2f& right) const
	{
		return !(*this == right);
	}

	Vector2f reflectAround(const Vector2f& vec) const
	{
		return (vec * (2 * dot(vec) / vec.lengthSquared()) - *this);
	}
	Vector2f reflectOn(const Vector2f& mirror) const
	{
		return (*this - mirror * (2 * dot(mirror) / mirror.lengthSquared()));
	}

	float dot(const Vector2f& right) const
	{
		return x*right.x + y*right.y;
	}
	Vector2f cross() const
	{
		return Vector2f(-y, x);
	}
	
	float lengthSquared() const
	{
		return (x*x + y*y);
	}
	float length() const
	{
		return sqrtf(lengthSquared());
	}

	Vector2f normalized() const
	{
		return *this / length();
	}
	void normalize()
	{
		*this = normalized();
	}

	void setLength(float _length)
	{
		normalize();
		this->operator*=(_length);
	}
};

#endif