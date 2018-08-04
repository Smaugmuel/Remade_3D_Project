#ifndef DEFERRED_SINGLE_COLOR_SHADER_GROUP_HPP
#define DEFERRED_SINGLE_COLOR_SHADER_GROUP_HPP
#include "../ShaderGroup.hpp"
//#include "../../../Math/Vector3.hpp"

#include <DirectXMath.h>
#include <string>

struct ID3D11Buffer;

class DeferredSingleColorShaderGroup final : public ShaderGroup
{
public:
	DeferredSingleColorShaderGroup();
	~DeferredSingleColorShaderGroup();

	bool Initialize(ID3D11Device* device) final override;

	void SetupShaders(ID3D11DeviceContext* deviceContext) final override;
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext);
	void SetupPerObjectBuffer(ID3D11DeviceContext* deviceContext);

private:
};

#endif