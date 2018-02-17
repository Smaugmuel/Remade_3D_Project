#include "TextureObject.hpp"

TextureObject::TextureObject()
{
}

TextureObject::~TextureObject()
{
}

bool TextureObject::Initialize()
{
	if (!Object::Initialize())
		return false;

	return true;
}

bool TextureObject::Initialize(const std::string & modelName, const std::string & textureName)
{
	if (!Object::Initialize())
		return false;

	m_modelName = modelName;
	m_textureName = textureName;

	return true;
}

void TextureObject::SetTextureName(const std::string & name)
{
	m_textureName = name;
}

const std::string & TextureObject::GetTextureName() const
{
	return m_textureName;
}