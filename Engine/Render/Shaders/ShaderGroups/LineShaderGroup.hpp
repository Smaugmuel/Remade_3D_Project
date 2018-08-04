#ifndef LINE_SHADER_GROUP_HPP
#define LINE_SHADER_GROUP_HPP
#include "../ShaderGroup.hpp"

class LineShaderGroup final : public ShaderGroup
{
public:
	LineShaderGroup();
	~LineShaderGroup();

	bool Initialize(ID3D11Device* device) final override;

	void SetupShaders(ID3D11DeviceContext* deviceContext) final override;
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext);
	void SetupPerObjectBuffer(ID3D11DeviceContext* deviceContext);
};

#endif