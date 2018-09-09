#ifndef LIGHT_MANAGER_HPP
#define LIGHT_MANAGER_HPP
#include "../Math/Vector3.hpp"

static const unsigned int MAX_NR_OF_LIGHTS = 4;

class LightManager final
{
public:
	LightManager();
	~LightManager();

private:
	struct LightBuffer
	{
		Vector3f m_lightPositions[MAX_NR_OF_LIGHTS];
		unsigned int m_nrOfLights;
	};

	int m_lightBufferID;
	LightBuffer m_lightBuffer;
};

#endif