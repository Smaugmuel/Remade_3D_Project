#ifndef PIXEL_SHADER_HPP
#define PIXEL_SHADER_HPP
#include "Shader.hpp"

struct ID3D11Device;
struct ID3D11PixelShader;

class PixelShader : public Shader
{
//protected:
public:
	PixelShader();
	virtual ~PixelShader();

//protected:
	bool Initialize(ID3D11Device* device, const char* shaderType);

	ID3D11PixelShader* GetShader() const;
private:
	ID3D11PixelShader* m_pixelShader;
};

#endif