#include "SceneManagerV2.hpp"
#include "MatrixManager.hpp"
#include "TextureManager.hpp"

SceneManagerV2::SceneManagerV2()
{
}

SceneManagerV2::~SceneManagerV2()
{
}

bool SceneManagerV2::Initialize(TextureManager * textureManager, MatrixManager * matrixManager)
{
	m_textureManager = textureManager;
	m_matrixManager = matrixManager;
	return true;
}

int SceneManagerV2::CreateObject()
{
	return m_objects.CreateElementAndGetIndex();
}

void SceneManagerV2::RemoveObject(int id)
{
	m_objects.RemoveElement(id);
}

void SceneManagerV2::SetObject(int id, const TexturedObjectV2 & object)
{
	m_objects.SetElement(id, object);
}

void SceneManagerV2::SetObjectTexture(int id, const std::string & textureName)
{
	if (TexturedObjectV2* object = m_objects.GetElement(id); object)
	{
		object->material.textureID = m_textureManager->LoadTexture(textureName);
	}
}

TexturedObjectV2* SceneManagerV2::GetObject(int id)
{
	return m_objects.GetElement(id);
}
