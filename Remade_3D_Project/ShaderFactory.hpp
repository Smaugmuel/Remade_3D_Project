#ifndef SHADER_FACTORY_HPP
#define SHADER_FACTORY_HPP
#include "Singleton.hpp"
#include <map>

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11Device;

struct ShaderGroupNew;

class ShaderFactory final : public Singleton<ShaderFactory>
{
	friend class Singleton<ShaderFactory>;

	typedef ShaderGroupNew* (*ShaderGroupFunction)(ID3D11Device*);
	typedef std::map<std::string, ShaderGroupFunction> FunctionMap;

	ShaderFactory();
	~ShaderFactory();

public:
	void Register(const std::string& name, ShaderGroupFunction function);
	ShaderGroupNew* Create(ID3D11Device* device, const std::string& name);

private:
	FunctionMap m_map;
};
#endif