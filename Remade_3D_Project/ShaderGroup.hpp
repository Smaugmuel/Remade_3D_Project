#ifndef SHADER_GROUP_HPP
#define SHADER_GROUP_HPP
#include <string>

struct ID3D11Device;
struct ID3D11DeviceContext;

class ShaderGroup
{
protected:
	ShaderGroup();
	virtual ~ShaderGroup();

public:
	virtual bool Initialize(ID3D11Device* device);
	virtual void SetupShaders(ID3D11DeviceContext* deviceContext);

protected:
	std::string m_vertexShaderName;
	std::string m_geometryShaderName;
	std::string m_pixelShaderName;
};

#endif