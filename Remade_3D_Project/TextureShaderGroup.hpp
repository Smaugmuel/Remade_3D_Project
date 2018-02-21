#ifndef TEXTURE_SHADER_GROUP_HPP
#define TEXTURE_SHADER_GROUP_HPP
#include "ShaderGroup.hpp"

#include <string>

struct ID3D11ShaderResourceView;

class TextureShaderGroup : public ShaderGroup
{
public:
	TextureShaderGroup();
	~TextureShaderGroup();

	bool Initialize(ID3D11Device* device) override;

	void SetupShaders(ID3D11DeviceContext* deviceContext) override;
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext);
	void SetupPerObjectBuffer(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture);

private:
	std::string m_samplerName;
};

#endif