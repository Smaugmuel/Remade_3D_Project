#include "LineObject.hpp"

LineObject::LineObject()
{
}

LineObject::~LineObject()
{
}

bool LineObject::Initialize()
{
	if (!Object::Initialize())
		return false;

	//m_modelName = "Line";
	m_color = Vector3f(0, 1, 1);

	return true;
}

void LineObject::SetColor(const Vector3f & color)
{
	m_color = color;
}

const Vector3f & LineObject::GetColor() const
{
	return m_color;
}

void LineObject::UpdateOBB()
{
}
