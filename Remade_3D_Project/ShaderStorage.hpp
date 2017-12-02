#ifndef SHADER_STORAGE_HPP
#define SHADER_STORAGE_HPP
#include "Singleton.hpp"

#include <map>
#include <memory>

#include "Shader.hpp"

struct ID3D11Device;

class ShaderStorage : public Singleton<ShaderStorage>
{
	friend class Singleton<ShaderStorage>;

	typedef std::map<std::string, std::unique_ptr<Shader>> ShaderMap;

private:
	ShaderStorage();
	~ShaderStorage();

public:

	bool CreateVertexShader(ID3D11Device* device, wchar_t* shaderName);
	bool CreateGeometryShader(ID3D11Device* device, wchar_t* shaderName);
	bool CreatePixelShader(ID3D11Device* device, wchar_t* shaderName);
	Shader* GetShader(wchar_t* shaderName) const;

private:
	ShaderMap m_shaderMap;
};

#endif