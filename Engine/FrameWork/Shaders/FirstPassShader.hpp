#ifndef FIRST_PASS_SHADER_HPP
#define FIRST_PASS_SHADER_HPP
#include "BaseShader.hpp"

class FirstPassShader : public BaseShader
{
public:
	FirstPassShader();
	~FirstPassShader();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

private:

};

#endif