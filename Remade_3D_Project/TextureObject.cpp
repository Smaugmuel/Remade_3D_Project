#include "TextureObject.hpp"
#include "TextureModel.hpp"
#include <d3d11.h>
#include "Texture.hpp"

TextureObject::TextureObject()
{
}

TextureObject::~TextureObject()
{
}

bool TextureObject::Initialize(ID3D11Device * device, const char * modelName)
{
	//m_model = std::unique_ptr<Model>(ModelFactory::Create(modelName, device));
	
	m_model = std::make_unique<TextureModel>();
	m_texture = std::make_unique<Texture>();
	
	if (!m_model->LoadFromFile(modelName))
	{
		return false;
	}
	if (!m_model->CreateVertexBuffer(device))
	{
		return false;
	}

	InitializeBaseClass();

	return true;
}

void TextureObject::Render(ID3D11DeviceContext * deviceContext)
{
	m_model->Render(deviceContext);
}

bool TextureObject::LoadTexture(ID3D11Device * device, const char * textureName)
{
	if (!m_texture->Initialize(device, textureName))
	{
		return false;
	}
	return true;
}

ID3D11ShaderResourceView * TextureObject::GetTexture()
{
	return m_texture->GetShaderResourceView();
}