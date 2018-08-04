#ifndef AABA_HPP
#define AABA_HPP
#include "Vector2.hpp"

struct AABA
{
	AABA();

	bool Intersects(const AABA& other) const;
	bool Contains(const AABA& other) const;
	bool Contains(const Vector2f& point) const;

	Vector2f center;
	Vector2f halfSides;
};

#endif