#include "TextureObject.hpp"

TextureObject::TextureObject()
{
}

TextureObject::~TextureObject()
{
}

bool TextureObject::Initialize()
{
	//m_model = std::unique_ptr<Model>(ModelFactory::Create(modelName, device));
	
	//m_model = std::make_unique<TextureModel>();
	//m_texture = std::make_unique<Texture>();
	//
	//if (!m_model->LoadFromFile(modelName))
	//{
	//	return false;
	//}
	//if (!m_model->CreateVertexBuffer(device))
	//{
	//	return false;
	//}

	InitializeBaseClass();

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