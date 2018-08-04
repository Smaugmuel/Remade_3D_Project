#include "RenderManager.hpp"

#include "../FrameWork/Direct3D.hpp"

#include "../Buffers/ConstantBufferStorage.hpp"
#include "Shaders/ShaderStorage.hpp"
#include "Shaders/ShaderManager.hpp"

#include "../Objects/Models/SingleColorModel.hpp"
#include "../Objects/Models/TextureModel.hpp"
#include "../Objects/Models/ModelStorage.hpp"

#include "../Objects/Objects/SingleColorObject.hpp"
#include "../Objects/Objects/TextureObject.hpp"

#include "../Objects/Textures/TextureStorage.hpp"

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
			model->SetupRender();

			for (auto& object : modelElement.second.elements)
			{
				// For each object using this model, set that object's world matrix and render it
				bufferStorage->SetVSWorldMatrix(deviceContext, object->GetWorldMatrix());
				model->Render();
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
		model->SetupRender();

		for (auto& object : modelElement.second.elements)
		{
			// For each object using this model, set that object's world matrix and color, and render it
			bufferStorage->SetVSWorldMatrix(deviceContext, object->GetWorldMatrix());
			bufferStorage->SetVSColor(deviceContext, object->GetColor());
			model->Render();
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