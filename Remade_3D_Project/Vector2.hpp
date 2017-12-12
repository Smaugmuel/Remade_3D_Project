#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <DirectXMath.h>

template<typename Type>
struct Vector2 final
{
	Type x;
	Type y;

	Vector2() : x(0), y(0)
	{
	}
	Vector2(const Type& pX, const Type& pY) : x(pX), y(pY)
	{
	}
	Vector2(const Vector2<int>& right) : x((Type)right.x), y((Type)right.y)
	{
	}
	Vector2(const Vector2<float>& right) : x((Type)right.x), y((Type)right.y)
	{
	}
	Vector2(const DirectX::XMFLOAT2& right) : x(right.x), y(right.y)
	{
	}
	~Vector2()
	{
	}

	Vector2<Type>& operator=(const Vector2<Type>& right)
	{
		x = right.x;
		y = right.y;

		return *this;
	}
	Vector2<Type> operator+=(const Vector2<Type>& right)
	{
		x += right.x;
		y += right.y;

		return *this;
	}
	Vector2<Type> operator-=(const Vector2<Type>& right)
	{
		x -= right.x;
		y -= right.y;

		return *this;
	}
	Vector2<Type> operator*=(const Type& coefficient)
	{
		x *= coefficient;
		y *= coefficient;

		return *this;
	}

	Vector2<Type> operator+(const Vector2<Type>& right) const
	{
		return Vector2<Type>(x + right.x, y + right.y);
	}
	Vector2<Type> operator-(const Vector2<Type>& right) const
	{
		return Vector2<Type>(x - right.x, y - right.y);
	}
	Vector2<Type> operator*(const Type& coefficient) const
	{
		return Vector2<Type>(x * coefficient, y * coefficient);
	}
	bool operator==(const Vector2<Type>& right) const
	{
		return right.x == x && right.y == y;
	}

	Vector2<Type> reflectAround(const Vector2<Type>& vec) const
	{
		return (vec * (2 * this->dot(vec) / vec.lengthSquared()) - *this);
	}
	Vector2<Type> reflectOn(const Vector2<Type>& mirror) const
	{
		return (*this - mirror * (2 * this->dot(mirror) / mirror.lengthSquared()));
	}

	float dot(const Vector2<Type>& right) const
	{
		return x*right.x + y*right.y;
	}
	Vector2<Type> cross() const
	{
		return Vector2<Type>(-y, x);
	}
	void normalize()
	{
		Type len = length();

		if (len == 0.0f)
			return;

		Type lenDiv = 1 / len;
		x *= lenDiv;
		y *= lenDiv;
	}

	float length() const
	{
		return std::sqrtf(lengthSquared());
	}
	float lengthSquared() const
	{
		return (x*x + y*y);
	}
	void setLength(const Type& _length)
	{
		normalize();
		this->operator*=(_length);
	}
};

typedef Vector2<float> Vector2f;
typedef Vector2<int> Vector2i;

#endif