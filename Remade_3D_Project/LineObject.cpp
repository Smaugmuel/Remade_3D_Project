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

	m_modelName = "Line";

	return true;
}

void LineObject::UpdateOBB()
{
}
