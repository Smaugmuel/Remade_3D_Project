#ifndef DEFERRED_LIGHT_MULTIPLE_LIGHTS_SHADER_GROUP_HPP
#define DEFERRED_LIGHT_MULTIPLE_LIGHTS_SHADER_GROUP_HPP
#include "Vector3.hpp"
#include <DirectXMath.h>
#include "SystemInformation.hpp"

#include <string>

//template<typename BufferData> struct ConstantBuffer;

struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11ShaderResourceView;

class DeferredLightMultipleLightsShaderGroup
{
	struct Light
	{
		Vector3f position;
		float intensity;
	};

	struct PS_PerFrameBuffer
	{
		Light lights[MAX_NR_OF_LIGHTS];
	};

public:
	DeferredLightMultipleLightsShaderGroup();
	~DeferredLightMultipleLightsShaderGroup();

	bool Initialize(ID3D11Device* device);

	void SetupShaders(ID3D11DeviceContext* deviceContext);
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext, unsigned int nrOfResources, ID3D11ShaderResourceView** resources, Vector3f lightPositions[MAX_NR_OF_LIGHTS], float lightIntensitys[MAX_NR_OF_LIGHTS]);

private:
	std::string m_vertexShaderName;
	std::string m_pixelShaderName;

	std::string m_samplerName;

	//ConstantBuffer<PS_PerFrameBuffer>* m_ps_per_frame_buffer;

	ID3D11Buffer* m_psPerFrameBuffer;
};

#endif