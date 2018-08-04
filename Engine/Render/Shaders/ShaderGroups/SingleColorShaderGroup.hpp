#ifndef SINGLE_COLOR_SHADER_GROUP_HPP
#define SINGLE_COLOR_SHADER_GROUP_HPP
#include "../ShaderGroup.hpp"

class SingleColorShaderGroup final : public ShaderGroup
{
public:
	SingleColorShaderGroup();
	~SingleColorShaderGroup();

	bool Initialize(ID3D11Device* device) final override;

	void SetupShaders(ID3D11DeviceContext* deviceContext) final override;
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext);
	void SetupPerObjectBuffer(ID3D11DeviceContext* deviceContext);

private:
};

#endif