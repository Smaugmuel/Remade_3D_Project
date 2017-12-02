#ifndef SHADER_HPP
#define SHADER_HPP

struct ID3D11Device;

class Shader
{
protected:
	Shader();
public:
	virtual ~Shader();

public:
	virtual bool Initialize(ID3D11Device* device, const char* shaderType) = 0;

//protected:
//	ID3D11Device* m_device;
};

#endif