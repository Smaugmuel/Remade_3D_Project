#include "SingleColorObject.hpp"

SingleColorObject::SingleColorObject()
{
}

SingleColorObject::~SingleColorObject()
{
}

bool SingleColorObject::Initialize()
{
	if (!Object::Initialize())
		return false;

	m_color = Vector3f(1, 0, 0);


	return true;
}

bool SingleColorObject::Initialize(const std::string & modelName, const Vector3f & color)
{
	if (!Object::Initialize())
		return false;

	m_modelName = modelName;
	m_color = color;

	return true;
}

void SingleColorObject::SetColor(const Vector3f & color)
{
	m_color = color;
}

void SingleColorObject::SetColor(float r, float g, float b)
{
	SetColor(Vector3f(r, g, b));
}

const Vector3f & SingleColorObject::GetColor() const
{
	return m_color;
}