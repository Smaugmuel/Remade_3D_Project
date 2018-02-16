#ifndef HUD_SHADER_GROUP_HPP
#define HUD_SHADER_GROUP_HPP
#include <string>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11ShaderResourceView;

class HUDShaderGroup
{
public:
	HUDShaderGroup();
	~HUDShaderGroup();

	bool Initialize(ID3D11Device* device);

	void SetupShaders(ID3D11DeviceContext* deviceContext);
	void SetupPerObjectBuffer(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture);

private:
	std::string m_vertexShaderName;
	std::string m_pixelShaderName;

	std::string m_samplerName;
};

#endif