#include "RenderManager.hpp"

#include "ModelStorage.hpp"
#include "TextureStorage.hpp"
#include "ShaderStorage.hpp"
#include "ShaderManager.hpp"

RenderManager* Singleton<RenderManager>::s_instance = nullptr;

RenderManager::RenderManager()
{
}

RenderManager::~RenderManager()
{
}

void RenderManager::Render()
{
	unsigned int nrOfShaders = 0;
	unsigned int nrOfTextures = 0;
	unsigned int nrOfModels = 0;

	for (unsigned int i = 0; i < nrOfShaders; i++)
	{
		for (unsigned int j = 0; j < nrOfTextures; j++)
		{
			for (unsigned int k = 0; k < nrOfModels; k++)
			{

			}
		}
	}
}

void RenderManager::ReceiveEvent(const Event & e)
{
}
