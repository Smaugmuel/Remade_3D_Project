#ifndef HUD_SHADER_GROUP_HPP
#define HUD_SHADER_GROUP_HPP
#include "ShaderGroup.hpp"

#include <string>

struct ID3D11ShaderResourceView;

class HUDShaderGroup : public ShaderGroup
{
public:
	HUDShaderGroup();
	~HUDShaderGroup();

	bool Initialize(ID3D11Device* device) override;

	void SetupShaders(ID3D11DeviceContext* deviceContext) override;
	void SetupPerObjectBuffer(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture);

private:
	std::string m_samplerName;
};

#endif