#ifndef POINT_LIGHT_MANAGER_HPP
#define POINT_LIGHT_MANAGER_HPP
#include "Singleton.hpp"

#include "PointLight.hpp"
#include <vector>
#include <memory>

class PointLightManager : public Singleton<PointLightManager>
{
	friend class Singleton<PointLightManager>;
	PointLightManager();
	~PointLightManager();

public:
	bool AddPointLight();
	bool RemovePointLight(unsigned int i);

	PointLight * GetPointLight(unsigned int i);
	const std::vector<std::unique_ptr<PointLight>>& GetPointLights() const;

	int GetNrOfPointLights() const;

private:
	std::vector<std::unique_ptr<PointLight>> m_pointLights;
};

#endif
