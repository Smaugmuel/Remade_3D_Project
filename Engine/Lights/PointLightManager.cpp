#include "PointLightManager.hpp"
#include "../Lights/LightSettings.hpp"

PointLightManager* Singleton<PointLightManager>::s_instance = nullptr;

PointLightManager::PointLightManager()
{
}

PointLightManager::~PointLightManager()
{
}

bool PointLightManager::AddPointLight()
{
	if (m_pointLights.size() >= MAX_NR_OF_LIGHTS)
		return false;

	m_pointLights.push_back(std::make_unique<PointLight>());

	return true;
}

bool PointLightManager::RemovePointLight(unsigned int i)
{
	if (i < 0 || i >= m_pointLights.size())
		return false;
	
	m_pointLights.erase(m_pointLights.begin() + i);

	return true;
}

PointLight * PointLightManager::GetPointLight(unsigned int i)
{
	return m_pointLights[i].get();
}

const std::vector<std::unique_ptr<PointLight>>& PointLightManager::GetPointLights() const
{
	return m_pointLights;
}

unsigned int PointLightManager::GetNrOfPointLights() const
{
	return m_pointLights.size();
}
