#ifndef SHADER_STORAGE_HPP
#define SHADER_STORAGE_HPP
#include "Singleton.hpp"

#include <map>
#include <string>

#pragma comment (lib, "d3dcompiler.lib")


struct ID3D11Device;

struct ID3D11VertexShader;
struct ID3D11GeometryShader;
struct ID3D11PixelShader;

struct ID3D11InputLayout;

struct D3D11_INPUT_ELEMENT_DESC;


class ShaderStorage : public Singleton<ShaderStorage>
{
	friend class Singleton<ShaderStorage>;

	struct Description
	{
		D3D11_INPUT_ELEMENT_DESC* elements;
		unsigned int n;
	};

	typedef std::map<std::string, ID3D11VertexShader*> VertexShaderMap;
	typedef std::map<std::string, ID3D11GeometryShader*> GeometryShaderMap;
	typedef std::map<std::string, ID3D11PixelShader*> PixelShaderMap;
	
	typedef std::map<std::string, std::string> LinkerMap;
	typedef std::map<std::string, Description> DescriptionMap;
	typedef std::map<std::string, ID3D11InputLayout*> LayoutMap;

	ShaderStorage();
	~ShaderStorage();

public:
	bool CreateVertexShader(ID3D11Device* device, std::string name);
	bool CreateGeometryShader(ID3D11Device* device, std::string name);
	bool CreatePixelShader(ID3D11Device* device, std::string name);

	ID3D11VertexShader* GetVertexShader(std::string name);
	ID3D11GeometryShader* GetGeometryShader(std::string name);
	ID3D11PixelShader* GetPixelShader(std::string name);

	ID3D11InputLayout* GetInputLayout(std::string name);

	bool HasVertexShader(std::string name) const;
	bool HasGeometryShader(std::string name) const;
	bool HasPixelShader(std::string name) const;


private:
	std::wstring ToWideString(const std::string& str) const;

	VertexShaderMap m_vertexShaders;
	GeometryShaderMap m_geometryShaders;
	PixelShaderMap m_pixelShaders;

	LinkerMap m_linker;
	DescriptionMap m_descriptions;
	LayoutMap m_layouts;
};

#endif