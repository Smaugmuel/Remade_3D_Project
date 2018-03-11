#ifndef DEFERRED_SINGLE_COLOR_SHADER_GROUP_HPP
#define DEFERRED_SINGLE_COLOR_SHADER_GROUP_HPP
#include "ShaderGroup.hpp"

#include <DirectXMath.h>
#include "Vector3.hpp"
#include <string>

struct ID3D11Buffer;

class DeferredSingleColorShaderGroup final : public ShaderGroup
{
public:
	DeferredSingleColorShaderGroup();
	~DeferredSingleColorShaderGroup();

	bool Initialize(ID3D11Device* device) override;

	void SetupShaders(ID3D11DeviceContext* deviceContext) override;
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext);
	void SetupPerObjectBuffer(ID3D11DeviceContext* deviceContext);

private:
};

#endif