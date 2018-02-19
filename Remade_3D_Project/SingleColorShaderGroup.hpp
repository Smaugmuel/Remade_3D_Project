#ifndef SINGLE_COLOR_SHADER_GROUP_HPP
#define SINGLE_COLOR_SHADER_GROUP_HPP
#include "ShaderGroup.hpp"

#include <DirectXMath.h>
#include "Vector3.hpp"
#include <string>

struct ID3D11Buffer;

class SingleColorShaderGroup : public ShaderGroup
{
	struct PS_PerFrameBuffer
	{
		// Stored as a float4 in shader to prevent (?) automatic padding
		Vector3f lightPosition;
		float lightIntensity;
	};

public:
	SingleColorShaderGroup();
	~SingleColorShaderGroup();

	bool Initialize(ID3D11Device* device) override;

	void SetupShaders(ID3D11DeviceContext* deviceContext) override;
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext);
	void SetupPerObjectBuffer(ID3D11DeviceContext* deviceContext);

private:
	ID3D11Buffer* m_psPerFrameBuffer;
};

#endif