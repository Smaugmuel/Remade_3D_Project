#include "RenderManager.hpp"

#include "Direct3D.hpp"

#include "ModelStorage.hpp"
#include "TextureStorage.hpp"
#include "ShaderStorage.hpp"
#include "ShaderManager.hpp"
#include "ConstantBufferStorage.hpp"

#include "TextureModel.hpp"
#include "TextureObject.hpp"

#include "SingleColorModel.hpp"
#include "SingleColorObject.hpp"

#include <algorithm>

RenderManager* Singleton<RenderManager>::s_instance = nullptr;

RenderManager::RenderManager()
{
}
RenderManager::~RenderManager()
{
	Reset();
}

void RenderManager::Reset()
{
	m_texturedObjects.elements.clear();
	m_singleColoredObjects.elements.clear();
}

void RenderManager::RenderTexturedObjects()
{
	ID3D11DeviceContext* deviceContext = Direct3D::Get()->GetDeviceContext();
	TextureStorage* textureStorage = TextureStorage::Get();
	ModelStorage* modelStorage = ModelStorage::Get();
	ConstantBufferStorage* bufferStorage = ConstantBufferStorage::Get();

	ShaderManager::Get()->SetShaderType(deviceContext, ShaderType::TEXTURE);

	for (auto& textureElement : m_texturedObjects.elements)
	{
		// For each texture, send that texture to shader
		ID3D11ShaderResourceView* texture = textureStorage->GetTexture(textureElement.first);
		deviceContext->PSSetShaderResources(0, 1, &texture);

		for (auto& modelElement : textureElement.second.elements)
		{
			// For each model using this texture, set that model to shader
			TextureModel* model = modelStorage->GetTextureModel(modelElement.first);
			model->SetupRender(deviceContext);

			for (auto& object : modelElement.second.elements)
			{
				// For each object using this model, set that object's world matrix and render it
				bufferStorage->SetVSWorldMatrix(deviceContext, object->GetWorldMatrix());
				model->Render(deviceContext);
			}
		}
	}
}
void RenderManager::RenderSingleColoredObjects()
{
	ID3D11DeviceContext* deviceContext = Direct3D::Get()->GetDeviceContext();
	ModelStorage* modelStorage = ModelStorage::Get();
	ConstantBufferStorage* bufferStorage = ConstantBufferStorage::Get();

	ShaderManager::Get()->SetShaderType(deviceContext, ShaderType::SINGLE_COLOR);

	for (auto& modelElement : m_singleColoredObjects.elements)
	{
		// For each model, set that model to shader
		SingleColorModel* model = modelStorage->GetSingleColorModel(modelElement.first);
		model->SetupRender(deviceContext);

		for (auto& object : modelElement.second.elements)
		{
			// For each object using this model, set that object's world matrix and color, and render it
			bufferStorage->SetVSWorldMatrix(deviceContext, object->GetWorldMatrix());
			bufferStorage->SetVSColor(deviceContext, object->GetColor());
			model->Render(deviceContext);
		}
	}
}

void RenderManager::AddTexturedObject(TextureObject * obj)
{
	m_texturedObjects.elements[obj->GetTextureName()].elements[obj->GetModelName()].elements.push_back(obj);
}
void RenderManager::AddSingleColoredObject(SingleColorObject * obj)
{
	m_singleColoredObjects.elements[obj->GetModelName()].elements.push_back(obj);
}

void RenderManager::ChangeTexture(TextureObject * obj, std::string newTextureName)
{
	std::vector<TextureObject*>& vec = m_texturedObjects.elements.at(obj->GetTextureName()).elements.at(obj->GetModelName()).elements;

	vec.erase(std::remove(vec.begin(), vec.end(), obj), vec.end());

	obj->SetTextureName(newTextureName);

	AddTexturedObject(obj);
}

void RenderManager::ReceiveEvent(const Event & e)
{
}