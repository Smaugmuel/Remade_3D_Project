#ifndef SHADER_CREATOR_HPP
#define SHADER_CREATOR_HPP
#include <string>
#include <vector>

enum class ShaderType
{
	VERTEX, GEOMETRY, PIXEL
};
enum class InputLayoutFormat
{
	FLOAT2, FLOAT3, FLOAT4
};

struct ShaderMacro
{
	std::string name;
	std::string definition;
};

struct ShaderDescription
{
	std::string name;
	ShaderType type;
	std::vector<ShaderMacro> macros;
};

struct InputLayoutElement
{
	std::string name;
	InputLayoutFormat format;
};

struct InputLayoutDescription
{
	std::vector<InputLayoutElement> elements;
};

struct Shader
{
	ShaderDescription shaderDescription;
	InputLayoutDescription inputLayoutDescription;
};

class ShaderCreator
{
public:
	ShaderCreator();
	~ShaderCreator();

private:
	std::vector<Shader> m_shaders;
};

#endif